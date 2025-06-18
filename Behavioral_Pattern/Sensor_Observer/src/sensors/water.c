#include "water.h"

void WaterSensorInit(WaterSensor *sensor) {
    PublisherInit(&sensor->base);
    sensor->isLeak = 0;
}
void WaterSensorTrigger(WaterSensor *sensor, int leak) {
    sensor->isLeak = leak;
    if (sensor->isLeak) {
        sensor->base.notifyToSubscriber(&sensor->base, "Water is leaked.\n");
    } else {
        sensor->base.notifyToSubscriber(&sensor->base, "Water is not leaked.\n");
    }
}