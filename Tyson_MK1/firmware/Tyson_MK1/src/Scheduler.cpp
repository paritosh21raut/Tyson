#include "Scheduler.h"

Scheduler::Scheduler()
{
}

bool Scheduler::every(uint32_t &timer, uint32_t period)
{
    uint32_t now = millis();

    if (now - timer >= period)
    {
        timer += period;

        return true;
    }

    return false;
}