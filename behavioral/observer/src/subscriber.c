#include "subscriber.h"
#include <stdio.h>
#include <string.h>
void updateEvent(Subscriber *sub, const char *event) {
    printf("Event: %s", event);

}

void SubscriberInit(Subscriber *sub) {
    memset(sub, 0, sizeof(Subscriber));
    sub->update = updateEvent;
}