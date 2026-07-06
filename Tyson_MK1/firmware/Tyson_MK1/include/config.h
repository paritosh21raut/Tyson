#pragma once

#include <Arduino.h>

/* ---------------- Robot Geometry ---------------- */

constexpr float WHEEL_RADIUS = 0.053f;
constexpr float TRACK_WIDTH  = 0.40f;

/* ---------------- Motor ---------------- */

constexpr float GEAR_RATIO = 180.0f;
constexpr float MAX_MOTOR_SPEED = 18000.0f;
constexpr float RAD_PER_SEC_TO_RPM = 9.5492966f;

/* ---------------- Robot Limits ---------------- */

constexpr float MAX_LINEAR_VELOCITY  = 5.0f;
constexpr float MAX_ANGULAR_VELOCITY = 3.0f;

/* ---------------- Communication ---------------- */

constexpr uint32_t BAUD_RATE = 9600;

constexpr char START_CHAR = '<';
constexpr char END_CHAR   = '>';
constexpr char DELIMITER  = ',';

constexpr uint8_t RX_BUFFER_SIZE = 64;

/* ---------------- Scheduler ---------------- */

constexpr uint16_t MOTOR_PERIOD_MS     = 20;
constexpr uint16_t ENCODER_PERIOD_MS   = 20;
constexpr uint16_t IMU_PERIOD_MS       = 10;
constexpr uint16_t SERIAL_TX_PERIOD_MS = 20;
constexpr uint16_t WATCHDOG_PERIOD_MS  = 10;

/* ---------------- Safety ---------------- */

constexpr uint16_t WATCHDOG_TIMEOUT_MS = 500;

/* ---------------- Motor IDs ---------------- */
constexpr uint8_t MOTOR_IDS[4] ={1, 2, 3, 4};

/*--------------- IMU ----------------*/
constexpr float MG_TO_MS2 = 0.00980665f;

constexpr float DEG_TO_RAD_FACTOR = 0.01745329252f;

/*---------------- Debug ----------------*/

constexpr bool DEBUG_SERIAL = false;