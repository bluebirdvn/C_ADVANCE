#ifndef _HVACSYS_H
#define _HVACSYS_H
#include "subscriber.h"



typedef struct {
    Subscriber base;    
}HvacSys;

void HvacSysInit(HvacSys *hvac);


#endif