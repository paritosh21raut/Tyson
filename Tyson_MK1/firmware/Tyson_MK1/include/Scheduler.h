#pragma once

#include <Arduino.h>

class Scheduler
{
public:

    Scheduler();

    bool every(uint32_t &timer, uint32_t period);

};