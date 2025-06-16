#ifndef _SENSORS_H
#define _SENSORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuresys.h"

typedef enum {
    SENSOR_TEMPERATURE,
    SENSOR_MOISTURE,
}SensorType;



typedef struct
{
    void (*init)(void);
    float (*readData)(void);
}Sensors;

Sensors* createSensor(SensorType type);


#endif