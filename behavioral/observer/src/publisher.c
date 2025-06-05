#include "publisher.h"
#include <stdio.h>
#include <string.h>
void Sub(Publisher *pub, Subscriber *sub);
void UnSub(Publisher *pub, Subscriber *sub);
void notify(Publisher *pub, const char *event); // Declare notify

void PublisherInit(Publisher *pub) {
    memset(pub, 0, sizeof(Publisher));
    pub->subscriberCount = 0;
    pub->subscribeToPub = Sub;
    pub->unSubscriberToPub = UnSub;
    pub->notifyToSubscriber = notify;
}

void Sub(Publisher *pub, Subscriber *sub) {
    if (pub->subscriberCount < MAXSUB) {
        pub->subscriber[pub->subscriberCount++] = sub;
        printf("Subscriber to PUB.\n");

    } else {
        printf("Reach max subscriber.\n");
    }
}

void UnSub(Publisher *pub, Subscriber *sub) {
    
    for (int i = 0; i < pub->subscriberCount; ++i) {
        if (pub->subscriber[i] == sub) {
            for (int j = i; j < pub->subscriberCount-1; ++j) {
                pub->subscriber[j] = pub->subscriber[j+1];
            }
            printf("Unsubscriber to PUB.\n");
            pub->subscriberCount--;
            pub->subscriber[pub->subscriberCount] = NULL;
            return;
        }
    }
}

void notify(Publisher *pub, const char *event) {
    for (int i = 0; i < pub->subscriberCount; ++i) {
        pub->subscriber[i]->update(pub->subscriber[i], event);
    }
}