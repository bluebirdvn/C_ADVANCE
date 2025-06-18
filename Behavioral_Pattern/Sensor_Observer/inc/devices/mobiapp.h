#ifndef _MOBIAPP_H
#define _MOBIAPP_H
#include "subscriber.h"
typedef struct {
    Subscriber base;
}MobiApp;
void MobiAppInit(MobiApp *mobi);
#endif