#ifndef _SENSORFACTORY_H
#define _SENSORFACTORY_H

#include "sensor.h"

typedef enum {
    SENSOR_TEMPERATURE,
    SENSOR_LIGHT,
    SENSOR_PRESSURE,
    SENSOR_HUMIDITY,
}SensorType;


Sensor* createSensor(SensorType type);

#endif