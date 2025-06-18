#include "pumping.h"
#include "config.h"
#include <string.h>
#include <stdio.h>
#include "log.h" // Thêm include



void pump_init(Pumping *pumping) {
    if (pumping == NULL) {
        log_message(LOG_LEVEL_ERROR, "Pumping is NULL in pump_init."); 
        return;
    }

    memset(pumping, 0, sizeof(Pumping));

    pumping->ledState = LED_NORMAL;
    pumping->pumpState = PUMP_OFF;
    pumping->last_time_pump_action = time(NULL);
    pumping->manual_watering_start_time = 0;
    SubscriberInit(&pumping->base);

    pumping->turn_pump_off = turn_pump_off;
    pumping->turn_pump_on = turn_pump_on;
    pumping->update_led_status = update_led_status;
    pumping->base.update = pumping_update_event; // Gán hàm update cụ thể cho pumping

    log_message(LOG_LEVEL_INFO, "Pumping system initialized."); 
}

RetCode_t turn_pump_on(Pumping *pumping) {
    if (pumping == NULL) {
        log_message(LOG_LEVEL_ERROR, "Pumping is NULL in turn_pump_on."); 
        return CODE_ERROR;
    }
    if (pumping->pumpState != PUMP_ON) {
        pumping->pumpState = PUMP_ON;
        pumping->last_time_pump_action = time(NULL);
        log_message(LOG_LEVEL_INFO, "Pump turned ON."); 
        return CODE_SUCCESS;
    } else {
        log_message(LOG_LEVEL_DEBUG, "Pump is already ON."); 
        return CODE_SUCCESS;
    }
}

RetCode_t turn_pump_off(Pumping *pumping) {
    if (pumping == NULL) {
        log_message(LOG_LEVEL_ERROR, "Pumping is NULL in turn_pump_off."); 
        return CODE_ERROR;
    }
    if (pumping->pumpState != PUMP_OFF) {
        pumping->pumpState = PUMP_OFF;
        pumping->last_time_pump_action = time(NULL);
        log_message(LOG_LEVEL_INFO, "Pump turned OFF."); 
        return CODE_SUCCESS;
    } else {
        log_message(LOG_LEVEL_DEBUG, "Pump is already OFF."); 
        return CODE_SUCCESS;
    }
}

void update_led_status(Pumping *pumping) {
    if (pumping == NULL) {
        log_message(LOG_LEVEL_ERROR, "Pumping is NULL in update_led_status."); 
        return;
    }

    LED_STATE new_led_state;
    if (pumping->pumpState == PUMP_ON) {
        new_led_state = LED_WATERING;
    } else if (current_sensor_data.moisture_percent < system_config_get()->min_moisture) {
        new_led_state = LED_LOW_MOISTURE_ALERT;
    } else {
        new_led_state = LED_NORMAL;
    }

    if (new_led_state != pumping->ledState) { 
        pumping->ledState = new_led_state;
        switch (pumping->ledState) {
            case LED_NORMAL: log_message(LOG_LEVEL_INFO, "[LED] State: NORMAL (Green)"); break;
            case LED_WATERING: log_message(LOG_LEVEL_INFO, "[LED] State: WATERING (Yellow)"); break;
            case LED_LOW_MOISTURE_ALERT: log_message(LOG_LEVEL_WARN, "[LED] State: LOW MOISTURE ALERT (Red)"); break;
            case LED_ERROR: log_message(LOG_LEVEL_ERROR, "[LED] State: ERROR (Blinking Red)"); break; 
        }
    }
}

void pumping_update_event(Subscriber *sub, const char *event) {
    (void)sub;
    log_message(LOG_LEVEL_DEBUG, "[PUMPING SUBSCRIBER] Received event: %s", event); 
}