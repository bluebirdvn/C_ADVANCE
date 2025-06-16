#include <stdio.h>
#include "moisture.h"

void mostureSensorInit() {
    printf("Pressure sensor init.\n");
}


float moistureSensorReadData() {
    return 80.0;
}

Sensors createMoistureSensor() {
    Sensors mostureSensor;
    mostureSensor.init = mostureSensorInit;
    mostureSensor.readData = moistureSensorReadData;
    return mostureSensor;
}

