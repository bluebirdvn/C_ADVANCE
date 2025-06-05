#ifndef _SMOKE_H
#define _SMOKE_H

#include "publisher.h"
typedef struct  {
    Publisher base;
    int detectedSmoke;
}SmokeSensor;

void SmokeSensorInit(SmokeSensor *sensor);
void SmokeSensorTrigger(SmokeSensor *sensor, int detected);

#endif
