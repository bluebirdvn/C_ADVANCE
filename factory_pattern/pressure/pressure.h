#ifndef _PRESSURE_H
#define _PRESSURE_H

#include "../sensorFactory/sensor.h"

void pressureSensorInit();

float pressureSensorReadData();

Sensor createPressureSensor();

#endif