#include "IMU.h"
#include "config.h"
#include "hardware.h"

IMU::IMU()
{
}

void IMU::begin()
{
}

void IMU::update()
{
    BNO::sAxisAnalog_t gyr = bno.getAxis(BNO::eAxisGyr);
    BNO::sAxisAnalog_t acc = bno.getAxis(BNO::eAxisLia);
    BNO::sQuaAnalog_t quat = bno.getQua();

    data_.timestamp = millis();

    /* Linear Acceleration (mg → m/s²) */

    data_.ax = acc.x * MG_TO_MS2;
    data_.ay = acc.y * MG_TO_MS2;
    data_.az = acc.z * MG_TO_MS2;

    /* Gyroscope (deg/s → rad/s) */

    data_.gx = gyr.x * DEG_TO_RAD_FACTOR;
    data_.gy = gyr.y * DEG_TO_RAD_FACTOR;
    data_.gz = gyr.z * DEG_TO_RAD_FACTOR;

    /* Quaternion */

    data_.qx = quat.x;
    data_.qy = quat.y;
    data_.qz = quat.z;
    data_.qw = quat.w;
}

const ImuData& IMU::getData() const
{
    return data_;
}