#ifndef MODESWITCH_H
#define MODESWITCH_h

#include "config.h"
#include "button.h"
#include "pumping.h"
#include <time.h>


void autoModeImplement(Pumping *pump, SystemConfig *system);
void manualModeImplement(Pumping *pump, SystemConfig *system);


#endif