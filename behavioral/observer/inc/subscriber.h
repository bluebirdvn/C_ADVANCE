#ifndef _SUBSCRIBER_H
#define _SUBSCRIBER_H
struct Subscriber_struct;
typedef struct Subscriber_struct{
    void (*update)(struct Subscriber_struct *sub, const char *event);   
}Subscriber;

void updateEvent(Subscriber *sub, const char *event);
void SubscriberInit(Subscriber *sub);
#endif
