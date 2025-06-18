#ifndef PUMPING_H
#define PUMPING_H

#include "subscriber.h"
#include "config.h"
#include <time.h>


typedef struct PumpingStruct{
    Subscriber base;
    PUMP_STATE pumpState;
    LED_STATE ledState;
    RetCode_t (*turn_pump_off)(struct PumpingStruct *pumping);
    RetCode_t (*turn_pump_on)(struct PumpingStruct *pumping);
    void(*update_led_status)(struct PumpingStruct *pumping);
    time_t last_time_pump_action;
    time_t manual_watering_start_time; 
} Pumping;
void pump_init(Pumping *pumping);
RetCode_t turn_pump_on(Pumping *pumping);
RetCode_t turn_pump_off(Pumping *pumping);
void update_led_status(Pumping *pumping);
void pumping_update_event(Subscriber *sub, const char *event);

#endif