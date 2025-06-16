#include "sensors.h"
#include "moisture.h"
#include "temperature.h"
Sensors* createSensor(SensorType type) {
    Sensors* sensor = (Sensors*)malloc(sizeof(Sensors));
    if (sensor == NULL) {
        return NULL;
    }

    switch (type) {
        case SENSOR_TEMPERATURE:
            *sensor = createTemperatureSensor();
            break;
        case SENSOR_MOISTURE:
            *sensor = createMoistureSensor();
            break;
        default:
            free(sensor);
            return NULL;
        
    }
    return sensor;
}
