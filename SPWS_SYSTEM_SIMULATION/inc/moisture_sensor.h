#ifndef MOISTURE_SENSOR_H
#define MOSITURE_SENSOR_H
#include "sensor.h"
#include "publisher.h"
#include "sensor_factory.h"


void moistureSensorInit(Sensors *self);

RetCode_t moistureSensorReadData(Sensors *self);

Sensors *createMoistureSensor();

#endif