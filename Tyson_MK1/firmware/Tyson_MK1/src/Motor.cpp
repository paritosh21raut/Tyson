#include "Motor.h"

#include "config.h"
#include "hardware.h"

Motor::Motor()
{
}

void Motor::begin()
{
}

void Motor::setCommand(const MotorCommand& command)
{
    command_ = command;
    last_command_time_ = millis();
}

void Motor::computeWheelSpeeds()
{
    float omega_left = (command_.linear_velocity - (TRACK_WIDTH * command_.angular_velocity) / 2.0f) / WHEEL_RADIUS;

    float omega_right = (command_.linear_velocity + (TRACK_WIDTH * command_.angular_velocity) / 2.0f) / WHEEL_RADIUS;

    left_.speed = omega_left * RAD_PER_SEC_TO_RPM * GEAR_RATIO;
    right_.speed = omega_right * RAD_PER_SEC_TO_RPM * GEAR_RATIO;

    left_.speed = constrain(left_.speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    right_.speed = constrain(right_.speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
}

void Motor::updateDirection()
{
    left_.direction = (left_.speed >= 0.0f) ? 0 : 1;
    right_.direction = (right_.speed >= 0.0f) ? 1 : 0;

    if (left_.direction != left_.previous_direction)
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            rmcs.Enable_Digital_Mode(MOTOR_IDS[i],left_.direction);
        }

        left_.previous_direction = left_.direction;
    }

    if (right_.direction != right_.previous_direction)
    {
        for (uint8_t i = 2; i < 4; i++)
        {
            rmcs.Enable_Digital_Mode(MOTOR_IDS[i],right_.direction);
        }

        right_.previous_direction = right_.direction;
    }
}

void Motor::driveMotors()
{
    for (uint8_t i = 0; i < 2; i++)
    {
        rmcs.Speed(MOTOR_IDS[i],fabs(left_.speed));
        rmcs.Speed(MOTOR_IDS[i + 2],fabs(right_.speed));
    }
}

void Motor::update()
{
    computeWheelSpeeds();

    updateDirection();

    driveMotors();
}

void Motor::stop()
{
    for (uint8_t i = 0; i < 2; i++)
    {
        rmcs.Disable_Digital_Mode(MOTOR_IDS[i],left_.direction);
        rmcs.Disable_Digital_Mode(MOTOR_IDS[i + 2],right_.direction);
    }

}

uint32_t Motor::lastCommandTime() const
{
    return last_command_time_;
}