#pragma once

#include <Arduino.h>

class Safety
{
public:

    Safety();

    void begin();

    void update(uint32_t last_command_time);

    bool emergencyStop() const;

private:

    bool emergency_stop_ = false;
};