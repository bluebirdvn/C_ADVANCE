#ifndef _MOISTURE_H
#define _MOISTURE_H

#include "sensors.h"

void pressureSensorInit();

float pressureSensorReadData();

Sensor createPressureSensor();

#endif