#ifndef SENSOR_H
#define SENSOR_H

#include "publisher.h"
#include "config.h"

typedef struct SensorsStruct{
    Publisher base;
    void (*init)(struct SensorsStruct *self);
    RetCode_t (*readData)(struct SensorsStruct *self);
} Sensors;


#endif
