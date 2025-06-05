#include <stdio.h>
#include "humidity.h"
#include "../sensorFactory/sensor.h"

void humiditySensorInit() {
    printf("Humidity sensor init.\n");
}


float humiditySensorReadData() {
    return 80.0;
}

Sensor createHumiditySensor() {
    Sensor humiditySensor;
    humiditySensor.init = humiditySensorInit;
    humiditySensor.readData = humiditySensorReadData;
    return humiditySensor;
}
