#ifndef _HUMIDITY_H
#define _HUMIDITY_H
#include "../sensorFactory/sensor.h"


void humiditySensorInit();

float humiditySensorReadData();

Sensor createHumiditySensor();

#endif