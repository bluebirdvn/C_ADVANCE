#include "light.h"

void LightSensorInit(LightSensor *sensor) {
    PublisherInit(&sensor->base);
    sensor->isOn = 0;
}
void LightSensorTrigger(LightSensor *sensor, int On) {
    sensor->isOn = On;
    if (sensor->isOn == 1) {
    sensor->base.notifyToSubscriber(&sensor->base, "Light is Open.\n");
    } else {
    sensor->base.notifyToSubscriber(&sensor->base, "Light is Off.\n");
    }
}
