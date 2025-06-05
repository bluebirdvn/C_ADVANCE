#include "hvacsys.h"

void HvacSysInit(HvacSys *hvac) {
    SubscriberInit(&hvac->base);
}