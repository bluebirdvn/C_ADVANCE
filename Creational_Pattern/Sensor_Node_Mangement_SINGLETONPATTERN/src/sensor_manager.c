#include "sensor_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static SensorManager* SensorManagerInstance = NULL;

static void addSensorImplement(SensorManager* self, SensorNode* sensor) {
    if (!self || !sensor) {
        fprintf(stderr, "Error: SensorManager or SensorNode is NULL\n");
        return;
    }

    if (self->sensors_count < MAX_NODESENSOR) {
        self->sensors[self->sensors_count++] = sensor;
        printf("SensorNode %d added to SensorManager.\n", sensor->id);
    } else {
        printf("SensorManager: Can't add more Node. Limit reached.\n");
    }
}

static int collectDataToBufferImplement(SensorManager *self) {
    if (!self) {
        fprintf(stderr, "Error: SensorManager is NULL\n");
        return -1;
    }

    self->internalDataBuffer[0] = '\0';
    char temp[MAX_DATA_STRING];

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(temp, sizeof(temp), "Data collect at: %Y-%m-%d %H:%M:%S\n", tm_info);
    strcat(self->internalDataBuffer, temp);

    int collected = 0;
    for (int i = 0; i < self->sensors_count; ++i) {
        if (self->sensors[i]) {
            int data = getData(self->sensors[i]);
            snprintf(temp, MAX_DATA_STRING, "Sensor ID %d: DATA = %d\n", self->sensors[i]->id, data);
            strcat(self->internalDataBuffer, temp);
            collected++;
        }
    }

    return collected;
}

static void pushBufferToStorageImplement(SensorManager* self, const char* filename) {
    if (!self) {
        fprintf(stderr, "Error: SensorManager is NULL\n");
        return;
    }

    if (strlen(self->internalDataBuffer) == 0) {
        printf("SensorManager: Internal buffer is empty\n");
        return;
    }

    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s", self->internalDataBuffer);
    fprintf(file, "-------------------------------\n");
    fclose(file);
    printf("Data has been pushed to file %s\n", filename);
}

static void initSensorManager(SensorManager *sensorManager) {
    sensorManager->sensors_count = 0;
    for (int i = 0; i < MAX_NODESENSOR; ++i) {
        sensorManager->sensors[i] = NULL;
    }

    sensorManager->internalDataBuffer[0] = '\0';
    sensorManager->collectData = collectDataToBufferImplement;
    sensorManager->pushData = pushBufferToStorageImplement;
    sensorManager->addSensor = addSensorImplement;
}

SensorManager* SensorManagerGetInstance() {
    if (!SensorManagerInstance) {
        SensorManagerInstance = (SensorManager*)malloc(sizeof(SensorManager));
        if (!SensorManagerInstance) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        initSensorManager(SensorManagerInstance);
    }
    return SensorManagerInstance;
}

void destroySensorManagerInstance() {
    if (SensorManagerInstance) {
        free(SensorManagerInstance);
        SensorManagerInstance = NULL;
        printf("SensorManager instance destroyed.\n");
    }
}
