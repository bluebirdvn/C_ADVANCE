#include "publisher.h"
#include <stdio.h>
#include <string.h>
#include "log.h" // Thêm include

void Sub(Publisher *pub, Subscriber *sub);
void UnSub(Publisher *pub, Subscriber *sub);
void notify(Publisher *pub, const char *event);

void PublisherInit(Publisher *pub) {
    memset(pub, 0, sizeof(Publisher));
    pub->subscriberCount = 0;
    pub->subscribeToPub = Sub;
    pub->unSubscriberToPub = UnSub;
    pub->notifyToSubscriber = notify;
    log_message(LOG_LEVEL_DEBUG, "Publisher initialized."); 
}

void Sub(Publisher *pub, Subscriber *sub) {
    if (pub->subscriberCount < MAXSUB) {
        pub->subscriber[pub->subscriberCount++] = sub;
        log_message(LOG_LEVEL_INFO, "Subscriber added to Publisher. Current count: %d", pub->subscriberCount); 

    } else {
        log_message(LOG_LEVEL_WARN, "Publisher reached max subscribers (%d). Cannot add more.", MAXSUB); 
    }
}

void UnSub(Publisher *pub, Subscriber *sub) {
    
    for (int i = 0; i < pub->subscriberCount; ++i) {
        if (pub->subscriber[i] == sub) {
            for (int j = i; j < pub->subscriberCount-1; ++j) {
                pub->subscriber[j] = pub->subscriber[j+1];
            }
            pub->subscriberCount--;
            pub->subscriber[pub->subscriberCount] = NULL;
            log_message(LOG_LEVEL_INFO, "Subscriber removed from Publisher. Current count: %d", pub->subscriberCount); 
            return;
        }
    }
    log_message(LOG_LEVEL_WARN, "Attempted to unsubscribe a non-existent subscriber."); 
}

void notify(Publisher *pub, const char *event) {
    for (int i = 0; i < pub->subscriberCount; ++i) {
        pub->subscriber[i]->update(pub->subscriber[i], event); 
    }
}