#include "sensor_manager.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
int main() {
    srand(time(NULL));

    SensorManager* sm = SensorManagerGetInstance();

    SensorNode n1 = {1}, n2 = {2}, n3 = {3};
    sm->addSensor(sm, &n1);
    sm->addSensor(sm, &n2);
    sm->addSensor(sm, &n3);

    const char* file = "sensorData.txt";

    if (sm->collectData(sm) > 0) {
        sm->pushData(sm, file);
    }

    printf("\n--- Collecting and pushing again ---\n");
    sm->collectData(sm);
    sm->pushData(sm, file);

    destroySensorManagerInstance();

    return 0;
}
