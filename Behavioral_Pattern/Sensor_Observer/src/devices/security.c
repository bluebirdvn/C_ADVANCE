#include "security.h"

void SecuritySysInit(SecuritySys *security) {
    SubscriberInit(&security->base);
}