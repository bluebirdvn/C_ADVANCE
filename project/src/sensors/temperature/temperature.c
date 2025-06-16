#include "temperature.h"
#include <stdio.h>
void temperatureSensorInit() {
    printf("Temperature Sensor init.\n");
}

float temperatureSensorReadData() {
    return 45.00;
}

Sensors createTemperatureSensor() {
    Sensors temperatureSensor;
    temperatureSensor.init = temperatureSensorInit;
    temperatureSensor.readData = temperatureSensorReadData;
    return temperatureSensor;
}
