#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H
#include "publisher.h"
typedef struct {
    Publisher base;
    int highTemp;
}TemperatureSensor;

void TemperatureSensorInit(TemperatureSensor *sensor);
void TemperatureSensorTrigger(TemperatureSensor *sensor, int high);
#endif