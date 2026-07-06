#pragma once

#include <Arduino.h>

#include "types.h"

class IMU
{
public:

    IMU();

    void begin();

    void update();

    const ImuData& getData() const;

private:

    ImuData data_;
};