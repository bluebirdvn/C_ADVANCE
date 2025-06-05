#include "temperature.h"

void TemperatureSensorInit(TemperatureSensor *sensor) {
    PublisherInit(&sensor->base);
    sensor->highTemp = 0;
}
void TemperatureSensorTrigger(TemperatureSensor *sensor, int high) {
    sensor->highTemp = high;
    if (sensor->highTemp) {
        sensor->base.notifyToSubscriber(&sensor->base, "Temperature is high.\n");
    } else {
        sensor->base.notifyToSubscriber(&sensor->base, "Temperature is not high.\n");
    }
}