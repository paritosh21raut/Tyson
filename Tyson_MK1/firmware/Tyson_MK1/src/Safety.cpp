#include "Safety.h"

#include "config.h"

Safety::Safety()
{
}

void Safety::begin()
{
}

void Safety::update(uint32_t last_command_time)
{
    emergency_stop_ = (millis() - last_command_time) > WATCHDOG_TIMEOUT_MS;
}

bool Safety::emergencyStop() const
{
    return emergency_stop_;
}