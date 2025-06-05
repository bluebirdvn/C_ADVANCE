#include "light.h"
#include "mobiapp.h"
#include "publisher.h"
#include "subscriber.h"
#include "door.h"
#include "security.h"
#include "smoke.h"
#include "temperature.h"
#include "water.h"
#include "hvacsys.h"


int main() {
    LightSensor light;
    LightSensorInit(&light);
    DoorSensor door;
    DoorSensorInit(&door);
    SmokeSensor smoke;
    SmokeSensorInit(&smoke);
    WaterSensor water;
    WaterSensorInit(&water);
    MobiApp mobi;
    MobiAppInit(&mobi);
    light.base.subscribeToPub(&light.base, &mobi.base);
    door.base.subscribeToPub(&door.base, &mobi.base);
    smoke.base.subscribeToPub(&smoke.base, &mobi.base);
    water.base.subscribeToPub(&water.base, &mobi.base);

    LightSensorTrigger(&light, 1);
    LightSensorTrigger(&light, 0);
    DoorSensorTrigger(&door, 1);
    SmokeSensorTrigger(&smoke, 1);
    WaterSensorTrigger(&water, 1);

    light.base.unSubscriberToPub(&light.base, &mobi.base);
    return 0;
}