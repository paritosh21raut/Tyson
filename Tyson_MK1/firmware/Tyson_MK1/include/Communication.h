#pragma once

#include <Arduino.h>

#include "types.h"
#include "config.h"

class Communication
{
public:

    Communication();

    void begin();

    void receive();

    void transmit(const EncoderData& encoder,const ImuData& imu);

    bool commandAvailable() const;

    MotorCommand getCommand();

private:

    char rx_buffer_[RX_BUFFER_SIZE];

    uint8_t index_ = 0;

    bool packet_started_ = false;

    bool command_available_ = false;

    MotorCommand command_;

    void parsePacket();
};