#pragma once

#include <Arduino.h>

#include "types.h"

class Motor
{
public:

    Motor();

    void begin();

    void update();

    void stop();

    void setCommand(const MotorCommand& command);

    uint32_t lastCommandTime() const;

private:

    struct WheelState
    {
        float speed = 0.0f;
        bool direction = false;
        bool previous_direction = false;
    };

    MotorCommand command_;

    WheelState left_;
    WheelState right_;

    uint32_t last_command_time_ = 0;

    void computeWheelSpeeds();

    void updateDirection();

    void driveMotors();
};