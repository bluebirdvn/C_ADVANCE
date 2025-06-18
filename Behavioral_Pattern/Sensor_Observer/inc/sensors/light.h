#ifndef _LIGHT_H
#define _LIGHT_H
#include "publisher.h"
typedef struct {
    Publisher base;
    int isOn;
} LightSensor;

void LightSensorInit(LightSensor *sensor);
void LightSensorTrigger(LightSensor *sensor, int On);
#endif