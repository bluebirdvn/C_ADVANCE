#ifndef _DEVICES_H
#define _DEVICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuresys.h"


struct Devices_struct;
typedef struct Devices_struct{
    void (*update)(struct Devices_struct *sub, const char *event);   
}Devices;

void updateEvent(Devices *sub, const char *event);
void SubscriberInit(Devices *sub);


#endif