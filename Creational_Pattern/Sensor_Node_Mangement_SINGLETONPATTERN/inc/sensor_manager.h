#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor_node.h"

#define MAX_NODESENSOR 10
#define MAX_DATA_STRING 256

typedef struct SensorManagerINS SensorManager;

struct SensorManagerINS {
    SensorNode* sensors[MAX_NODESENSOR];
    int sensors_count;
    char internalDataBuffer[MAX_DATA_STRING*10];

    int (*collectData)(SensorManager* self);
    void (*pushData)(SensorManager* self, const char* filename);
    void (*addSensor)(SensorManager* self, SensorNode* sensor);
};

SensorManager* SensorManagerGetInstance(void);
void destroySensorManagerInstance(void);

#endif
