#ifndef _SECURITY_H
#define _SECURITY_H
#include "subscriber.h"



typedef struct {
    Subscriber base;    
}SecuritySys;

void SecuritySysInit(SecuritySys *security);


#endif