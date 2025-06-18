#include <stdio.h>
#include "pressure.h"

void pressureSensorInit() {
    printf("Pressure sensor init.\n");
}


float pressureSensorReadData() {
    return 80.0;
}

Sensor createPressureSensor() {
    Sensor pressureSensor;
    pressureSensor.init = pressureSensorInit;
    pressureSensor.readData = pressureSensorReadData;
    return pressureSensor;
}