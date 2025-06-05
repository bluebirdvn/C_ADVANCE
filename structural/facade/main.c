#include "facade/facade.h"
#include <stdio.h>

int main() {
    SmartHomeSys *smartHomeSys = createSmartHomeSys();

    goodMorningMode(smartHomeSys);

    awayMode(smartHomeSys);

    welcomeHomeMode(smartHomeSys);

    movieMode(smartHomeSys);

    goodNightMode(smartHomeSys);

    securityAlertOn(smartHomeSys);

    securityAlertOff(smartHomeSys);

    destroySmartHomeSys(smartHomeSys);

    return 0;

}