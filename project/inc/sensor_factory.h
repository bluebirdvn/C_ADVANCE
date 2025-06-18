#ifndef SENSOR_FACTORY_H
#define SENSOR_FACTORY_H


#include "sensor.h"

typedef enum {
    MOISTURE_SENSOR,
    TEMPERATURE_SENSOR
} SensorType;


Sensors *CreateSensor(SensorType type);


#endif