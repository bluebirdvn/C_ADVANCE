#include "temperature.h"
#include <stdio.h>
void temperatureSensorInit() {
    printf("Temperature Sensor init.\n");
}

float temperatureSensorReadData() {
    return 45.00;
}

Sensor createTemperatureSensor() {
    Sensor temperatureSensor;
    temperatureSensor.init = temperatureSensorInit;
    temperatureSensor.readData = temperatureSensorReadData;
    return temperatureSensor;
}
