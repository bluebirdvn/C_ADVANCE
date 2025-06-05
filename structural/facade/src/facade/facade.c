#include "facade/facade.h"
#include <stdio.h>
#include <stdlib.h>

SmartHomeSys *createSmartHomeSys() {
    SmartHomeSys* smartHomeSys = (SmartHomeSys*)malloc(sizeof(SmartHomeSys));
    smartHomeSys->hvac = createHvacSystem();
    smartHomeSys->light = createLightingSystem();
    smartHomeSys->security = createSecuritySystem();
    return smartHomeSys;
}

void destroySmartHomeSys(SmartHomeSys *smartHomeSys) {
    free(smartHomeSys->hvac);
    free(smartHomeSys->light);
    free(smartHomeSys->security);
    free(smartHomeSys);
    printf("Destructor smartHomeSys.\n");
}

void goodMorningMode(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->turnOn();
    smartHomeSys->light->turnOff();
    smartHomeSys->security->activeSecurity();
    printf("Good Morning Mode Entering...\n");

}

void awayMode(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->turnOff();
    smartHomeSys->light->turnOff();
    smartHomeSys->security->activeSecurity();
    printf("Away Mode Mode Entering...\n");    
}

void welcomeHomeMode(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->autoSet();
    smartHomeSys->light->setBrightness();
    smartHomeSys->security->disableSecurity();
    printf("Welcom Home Mode Entering...\n");
}

void movieMode(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->decreaseTemp();
    smartHomeSys->light->turnOff();
    smartHomeSys->security->activeSecurity();
    printf("Movie Mode Entering...\n");
}

void goodNightMode(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->autoSet();
    smartHomeSys->light->setBrightness();
    smartHomeSys->security->activeSecurity();
    printf("Good night Mode Entering...\n");
}

void securityAlertOn(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->turnOff();
    smartHomeSys->light->turnOn();
    smartHomeSys->security->turnAlarmOn();
    printf("Alert ON Mode Entering...\n");

}

void securityAlertOff(SmartHomeSys *smartHomeSys) {
    smartHomeSys->hvac->increaseTemp();
    smartHomeSys->light->setBrightness();
    smartHomeSys->security->turnAlarmOff();
    printf("Alert OFF Mode Entering...\n");

}

