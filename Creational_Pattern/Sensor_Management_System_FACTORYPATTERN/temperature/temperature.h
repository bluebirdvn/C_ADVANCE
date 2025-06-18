#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H

#include "../sensorFactory/sensor.h"

void temperatureSensorInit();

float temperatureSensorReadData();

Sensor createTemperatureSensor();

#endif