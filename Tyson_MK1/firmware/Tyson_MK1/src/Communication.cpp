#include "Communication.h"

#include "config.h"

Communication::Communication()
{
}

void Communication::begin()
{
}

bool Communication::commandAvailable() const
{
    return command_available_;
}

MotorCommand Communication::getCommand()
{
    command_available_ = false;
    return command_;
}

void Communication::receive()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (c == START_CHAR)
        {
            packet_started_ = true;
            index_ = 0;
            continue;
        }

        if (!packet_started_)
        {
            continue;
        }

        if (c == END_CHAR)
        {
            rx_buffer_[index_] = '\0';

            parsePacket();

            packet_started_ = false;

            index_ = 0;

            continue;
        }

        if (index_ < RX_BUFFER_SIZE - 1)
        {
            rx_buffer_[index_++] = c;
        }
    }
}

void Communication::transmit(const EncoderData& encoder, const ImuData& imu)
{
    Serial.print(START_CHAR);

    Serial.print(encoder.timestamp);
    Serial.print(DELIMITER);

    for (uint8_t i = 0; i < 4; i++)
    {
        Serial.print(encoder.ticks[i]);
        Serial.print(DELIMITER);
    }

    Serial.print(imu.ax); Serial.print(DELIMITER);
    Serial.print(imu.ay); Serial.print(DELIMITER);
    Serial.print(imu.az); Serial.print(DELIMITER);

    Serial.print(imu.gx); Serial.print(DELIMITER);
    Serial.print(imu.gy); Serial.print(DELIMITER);
    Serial.print(imu.gz); Serial.print(DELIMITER);

    Serial.print(imu.qx); Serial.print(DELIMITER);
    Serial.print(imu.qy); Serial.print(DELIMITER);
    Serial.print(imu.qz); Serial.print(DELIMITER);

    Serial.print(imu.qw);

    Serial.println(END_CHAR);
}

void Communication::parsePacket()
{
    char *comma = strchr(rx_buffer_, DELIMITER);

    if (comma == nullptr)
        return;

    *comma = '\0';

    command_.linear_velocity = atof(rx_buffer_);

    command_.angular_velocity = atof(comma + 1);

    command_available_ = true;
}