#include "subscriber.h"
#include <stdio.h>
#include <string.h>
#include "log.h" 

void updateEvent(Subscriber *sub, const char *event) {
    (void)sub;
    log_message(LOG_LEVEL_INFO, "Subscriber received event: %s", event); 
}

void SubscriberInit(Subscriber *sub) {
    memset(sub, 0, sizeof(Subscriber));
    sub->update = updateEvent;
    log_message(LOG_LEVEL_DEBUG, "Subscriber initialized."); 
}