#include <stdio.h>
#include "light.h"

void lightSensorInit() {
    printf("Humidity sensor init.\n");
}


float lightSensorReadData() {
    return 80.0;
}

Sensor createLightSensor() {
    Sensor lightSensor;
    lightSensor.init = lightSensorInit;
    lightSensor.readData = lightSensorReadData;
    return lightSensor;
}