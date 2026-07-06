#include "hardware.h"
#include <Wire.h>

SoftwareSerial rmcs_serial(2, 3);

RMCS2303 rmcs;

BNO bno(&Wire, 0x28);