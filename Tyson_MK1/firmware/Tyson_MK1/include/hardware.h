#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

#include <RMCS2303drive.h>

#include "DFRobot_BNO055.h"

typedef DFRobot_BNO055_IIC BNO;

extern SoftwareSerial rmcs_serial;

extern RMCS2303 rmcs;

extern BNO bno;