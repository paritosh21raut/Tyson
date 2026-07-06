#pragma once

#include <Arduino.h>

struct MotorCommand
{
    float linear_velocity = 0.0f;
    float angular_velocity = 0.0f;
};

struct EncoderData
{
    long ticks[4] = {0};

    uint32_t timestamp = 0;
};

struct ImuData
{
    float ax = 0;
    float ay = 0;
    float az = 0;

    float gx = 0;
    float gy = 0;
    float gz = 0;

    float qx = 0;
    float qy = 0;
    float qz = 0;
    float qw = 1;

    uint32_t timestamp = 0;
};