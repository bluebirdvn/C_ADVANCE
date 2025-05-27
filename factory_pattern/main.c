#include <stdio.h>
#include <stdlib.h>
#include "sensorFactory/sensor_factory.h"

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
    return 0;
}