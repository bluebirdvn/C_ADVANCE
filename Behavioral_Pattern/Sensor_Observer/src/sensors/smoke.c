#include "smoke.h"

void SmokeSensorInit(SmokeSensor *sensor) {
    PublisherInit(&sensor->base);
    sensor->detectedSmoke = 0;
}
void SmokeSensorTrigger(SmokeSensor *sensor, int detected) {
    sensor->detectedSmoke = detected;
    if (sensor->detectedSmoke) {
        sensor->base.notifyToSubscriber(&sensor->base, "Detected Smoke.\n");
    } else {
        sensor->base.notifyToSubscriber(&sensor->base, "Not detected Smoke.\n");
    }
}