#ifndef _LIGHT_H
#define _LIGHT_H

#include "../sensorFactory/sensor.h"

void lightSensorInit();

float lightSensorReadData();

Sensor createLightSensor();

#endif