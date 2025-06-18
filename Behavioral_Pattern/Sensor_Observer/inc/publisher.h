#ifndef _PUBLISHER_H
#define _PUBLISHER_H
#include "subscriber.h"

#define MAXSUB 10

typedef struct Publisher_struct{
    Subscriber *subscriber[MAXSUB];
    int subscriberCount;
    void (*subscribeToPub)(struct Publisher_struct *pub, Subscriber *sub);
    void (*unSubscriberToPub)(struct Publisher_struct *pub, Subscriber *sub);
    void (*notifyToSubscriber)(struct Publisher_struct *pub, const char *event);
}Publisher;

void PublisherInit(Publisher *pub);

#endif