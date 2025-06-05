#ifndef _DOOR_H
#define _DOOR_H
#include "publisher.h"
typedef struct {
    Publisher base;
    int isOpen;
} DoorSensor;

void DoorSensorInit(DoorSensor *sensor);
void DoorSensorTrigger(DoorSensor *sensor, int Open);
#endif