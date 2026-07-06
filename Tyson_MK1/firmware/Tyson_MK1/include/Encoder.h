#pragma once

#include <Arduino.h>

#include "types.h"

class Encoder
{
public:

    Encoder();

    void begin();

    void update();

    const EncoderData& getData() const; 
    
private:

    EncoderData data_;

    int32_t bias[4] = {0, 0, 0, 0};

    bool calibrated = false;

    uint32_t calibration_start = 0;
};