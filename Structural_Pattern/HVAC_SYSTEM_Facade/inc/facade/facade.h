#ifndef _FACADE_H
#define _FACADE_H

#include "lighting/lighting.h"
#include "hvacsystem/hvac.h"
#include "security/security.h"


typedef struct {
	HvacSystem *hvac;
	LightingSystem *light;
	SecuritySystem *security;
}SmartHomeSys;

SmartHomeSys *createSmartHomeSys();

void destroySmartHomeSys(SmartHomeSys *smartHomeSys);

void goodMorningMode(SmartHomeSys *smartHomeSys);

void awayMode(SmartHomeSys *smartHomeSys);

void welcomeHomeMode(SmartHomeSys *smartHomeSys);

void movieMode(SmartHomeSys *smartHomeSys);

void goodNightMode(SmartHomeSys *smartHomeSys);

void securityAlertOn(SmartHomeSys *smartHomeSys);

void securityAlertOff(SmartHomeSys *smartHomeSys);



#endif //_FACADE_H
