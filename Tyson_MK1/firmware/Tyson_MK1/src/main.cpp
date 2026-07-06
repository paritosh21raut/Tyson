#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "hardware.h"

#include "Motor.h"
#include "Encoder.h"
#include "IMU.h"
#include "Communication.h"
#include "Scheduler.h"
#include "Safety.h"

Motor motor;
Encoder encoder;
IMU imu;
Communication communication;
Scheduler scheduler;
Safety safety;

/*---------------- Timers ----------------*/

uint32_t motor_timer = 0;
uint32_t encoder_timer = 0;
uint32_t imu_timer = 0;
uint32_t serial_tx_timer = 0;
uint32_t watchdog_timer = 0;

void setup()
{
    Serial.begin(BAUD_RATE);

    Wire.begin();

    /*--------------- RMCS ----------------*/

    rmcs.Serial_selection(1);
    rmcs.Serial0(BAUD_RATE);
    rmcs.begin(&rmcs_serial, BAUD_RATE);

    /*--------------- BNO055 --------------*/

    bno.reset();

    while (bno.begin() != BNO::eStatusOK)
    {
        delay(1000);
    }

    /*--------------- Modules -------------*/

    motor.begin();
    motor.stop();

    encoder.begin();

    imu.begin();

    communication.begin();

    safety.begin();
}
void loop()
{
    communication.receive();

    if (communication.commandAvailable())
    {
        motor.setCommand(communication.getCommand());
    }

    // Encoder runs every loop
    encoder.update();

    if (scheduler.every(imu_timer, IMU_PERIOD_MS))
    {
        imu.update();
    }

    if (scheduler.every(motor_timer, MOTOR_PERIOD_MS))
    {
        motor.update();
    }

    if (scheduler.every(watchdog_timer, WATCHDOG_PERIOD_MS))
    {
        safety.update(motor.lastCommandTime());

        if (safety.emergencyStop())
        {
            motor.stop();
        }
    }

    if (scheduler.every(serial_tx_timer, SERIAL_TX_PERIOD_MS))
    {
        communication.transmit(
            encoder.getData(),
            imu.getData());
    }
}