#include "modeswitch.h"
#include "config.h"
#include "pumping.h"
#include <stdio.h> 
#include <time.h> 
#include "log.h"



static time_t last_sensor_check_time = 0; 
static time_t last_auto_watering_completion_time = 0; 

#define MIN_INTERVAL_BETWEEN_WATERINGS_SEC 300 

void autoModeImplement(Pumping *pump, SystemConfig *system) {
    if (pump == NULL || system == NULL) {
        log_message(LOG_LEVEL_ERROR, "NULL pointer in autoModeImplement."); 
        return; 
    }

    time_t timeNow = time(NULL);

    if (timeNow - last_sensor_check_time < system->sensor_check_interval_sec) {
        return; 
    }
    last_sensor_check_time = timeNow; 

    log_message(LOG_LEVEL_INFO, "[AUTO MODE] Current moisture: %d%%. Min: %d%%, Max: %d%%", 
                   current_sensor_data.moisture_percent, system->min_moisture, system->max_moisture); 

    PUMP_STATE prev_pump_state = pump->pumpState; 

    if (current_sensor_data.moisture_percent < system->min_moisture &&
        (timeNow - last_auto_watering_completion_time >= MIN_INTERVAL_BETWEEN_WATERINGS_SEC)) {
        if (pump->pumpState == PUMP_OFF) {
            log_message(LOG_LEVEL_INFO, "[AUTO MODE] Moisture too low (%d%% < %d%%), turning pump ON.", 
                       current_sensor_data.moisture_percent, system->min_moisture); 
            pump->turn_pump_on(pump); 
        }
    } 
    
    if (pump->pumpState == PUMP_ON) {
        
        if (current_sensor_data.moisture_percent >= system->max_moisture) {
            log_message(LOG_LEVEL_INFO, "[AUTO MODE] Moisture reached max threshold (%d%% >= %d%%), turning pump OFF.",
            current_sensor_data.moisture_percent, system->max_moisture); 
            pump->turn_pump_off(pump); 
        } else if (timeNow - pump->last_time_pump_action >= system->max_watering_time_sec) {
            log_message(LOG_LEVEL_INFO, "[AUTO MODE] Max watering time reached (%d sec), turning pump OFF.", 
            system->max_watering_time_sec); 
            pump->turn_pump_off(pump); 
        }
    }

    if (prev_pump_state == PUMP_ON && pump->pumpState == PUMP_OFF) {
        last_auto_watering_completion_time = timeNow;
        log_message(LOG_LEVEL_INFO, "[AUTO MODE] Pump just turned OFF, setting last auto watering completion time.");
    }
}

void manualModeImplement(Pumping *pump, SystemConfig *system) {
    if (pump == NULL || system == NULL) {
        log_message(LOG_LEVEL_ERROR, "NULL pointer in manualModeImplement."); 
        return; 
    }

    time_t timeNow = time(NULL);

    if (pump->pumpState == PUMP_ON && pump->manual_watering_start_time != 0) {
        if (timeNow - pump->manual_watering_start_time >= system->manual_watering_duration_sec) {
            log_message(LOG_LEVEL_INFO, "[MANUAL MODE] Manual watering time (%d sec) elapsed, turning pump OFF.", 
                       system->manual_watering_duration_sec); 
            pump->turn_pump_off(pump);
            pump->manual_watering_start_time = 0; 
        }
    }
}