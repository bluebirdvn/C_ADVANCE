#include <stdio.h>
#include "moisture_sensor.h"
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "publisher.h"
#include "log.h" 

#define MAX_BUFFER_SIZE 100 



void moistureSensorInit(Sensors *self) {
    if (self == NULL) {
        log_message(LOG_LEVEL_ERROR, "Moisture sensor init received NULL self."); 
        return;
    }
    PublisherInit(&self->base);
    log_message(LOG_LEVEL_INFO, "Moisture sensor init successful."); 
}

RetCode_t moistureSensorReadData(Sensors *self) {
    if (self == NULL) {
        log_message(LOG_LEVEL_ERROR, "moistureSensorReadData received NULL self."); 
        return CODE_ERROR;
    }

    int moisture = rand() % 100;
    current_sensor_data.moisture_percent = (uint8_t)moisture;

    char *msg = (char*)malloc(MAX_BUFFER_SIZE);
    
    if (msg == NULL) {
        log_message(LOG_LEVEL_ERROR, "Failed to allocate memory for moisture message."); 
        return CODE_ERROR;
    }
    
    snprintf(msg, MAX_BUFFER_SIZE, "MOISTURE=%d", moisture);
    self->base.notifyToSubscriber(&self->base, msg);

    free(msg);
    return CODE_SUCCESS;
}

Sensors *createMoistureSensor() {
    Sensors *sensor = (Sensors*)malloc(sizeof(Sensors));
    if (sensor == NULL) {
        log_message(LOG_LEVEL_ERROR, "Failed to allocate memory for Moisture sensor."); 
        return NULL;
    }

    sensor->init = moistureSensorInit;
    sensor->readData = moistureSensorReadData;

    log_message(LOG_LEVEL_INFO, "Moisture sensor object created successfully.");

    return sensor;
}
