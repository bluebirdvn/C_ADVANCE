#include "door.h"

void DoorSensorInit(DoorSensor *sensor) {
    PublisherInit(&sensor->base);
    sensor->isOpen = 0;
}
void DoorSensorTrigger(DoorSensor *sensor, int Open) {
    sensor->isOpen = Open;
    if (sensor->isOpen) {
        sensor->base.notifyToSubscriber(&sensor->base, "Door is open.\n");
    } else {
        sensor->base.notifyToSubscriber(&sensor->base, "Door is Close.\n");
    }
}