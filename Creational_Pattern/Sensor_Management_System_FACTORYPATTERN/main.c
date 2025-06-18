#include <stdio.h>
#include <stdlib.h>
#include "sensorFactory/sensorfactory.h"


int main() {
    /*
    create light sensor
    */
    Sensor* lightSensor = createSensor(SENSOR_LIGHT);

    if (lightSensor != NULL) {
        lightSensor->init();
        printf("Light: %.2f C\n", lightSensor->readData());
        
        free(lightSensor);
    }

    Sensor* humiditySensor = createSensor(SENSOR_HUMIDITY);

    if (humiditySensor != NULL) {
        humiditySensor->init();
        printf("humidity: %.2f C\n", humiditySensor->readData());

        free(humiditySensor);
    }   

    return 0;
}