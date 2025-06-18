#ifndef _WATER_H
#define _WATER_H

#include "publisher.h"
typedef struct  {
    Publisher base;
    int isLeak;
}WaterSensor;

void WaterSensorInit(WaterSensor *sensor);
void WaterSensorTrigger(WaterSensor *sensor, int leak);

#endif
