#include "Encoder.h"

#include "config.h"
#include "hardware.h"

Encoder::Encoder()
{
}

void Encoder::begin()
{
    calibration_start = millis();
}

void Encoder::update()
{
    static uint8_t index = 0;

    // Read one motor each update
    data_.timestamp = millis();

    data_.ticks[index] = rmcs.Position_Feedback(MOTOR_IDS[index]);

    // Reverse encoder direction if required
    if (index == 2 || index == 3)
        data_.ticks[index] *= -1;

    // -------------------------------------------------
    // Zero-offset calibration (run once after 5 seconds)
    // -------------------------------------------------

    if (!calibrated)
    {
        if (millis() - calibration_start >= 5000)
        {
            for (uint8_t i = 0; i < 4; i++)
            {
                bias[i] = data_.ticks[i];
            }

            calibrated = true;
        }

        index++;

        if (index >= 4)
            index = 0;

        return;
    }

    // --------------------------
    // Remove initial bias
    // --------------------------

    data_.ticks[index] -= bias[index];

    index++;

    if (index >= 4)
        index = 0;
}

const EncoderData& Encoder::getData() const
{
    return data_;
}