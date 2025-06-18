#include <stdlib.h>
#include <stdio.h>

#include "sensor_factory.h" 
#include "sensor.h"
#include "config.h"
#include "moisture_sensor.h"
#include "log.h" // Thêm include

Sensors *CreateSensor(SensorType type) {
    Sensors *sensor = NULL;

    switch (type) {
        case MOISTURE_SENSOR:
            sensor = createMoistureSensor(); 
            break;
        // case TEMPERATURE_SENSOR:
        //     sensor = CreateTemperatureSensor(); 
        //     break;
        default:
            log_message(LOG_LEVEL_ERROR, "Sensortype %d is not valid.", type); 
            return NULL; 
    }

    if (sensor == NULL) {
        log_message(LOG_LEVEL_ERROR, "Failed to create sensor of type %d.", type); 
        return NULL;
    }
    
    log_message(LOG_LEVEL_INFO, "Sensor factory created sensor of type %d.", type); 
    return sensor;
}