#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H

#include "sensors.h"

void temperatureSensorInit();

float temperatureSensorReadData();

Sensor createTemperatureSensor();

#endif