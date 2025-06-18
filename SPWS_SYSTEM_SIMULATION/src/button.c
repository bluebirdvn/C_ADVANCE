#include "button.h"
#include "config.h"
#include "pumping.h"
#include <stdio.h> 
#include <stdbool.h>
#include <time.h> 
#include "log.h" 

void buttons_init() {
    log_message(LOG_LEVEL_INFO, "[BUTTONS] Initialized. (Simulated input: 'a' for mode toggle, 'm' for manual water, 'x' for exit while loop)"); 
}

void button_check_events(Pumping *pumping) {
    char input;
    printf("\nPress 'a' for AUTO/MANUAL toggle, 'm' for MANUAL water: ");
    if (scanf(" %c", &input) != 1) { 
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        log_message(LOG_LEVEL_WARN, "Invalid input received, clearing buffer."); 
        return;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Xóa bộ đệm input

    SystemConfig* cfg = system_config_get();

    if (input == 'a') {
        if (cfg->mode == MODE_AUTO) {
            if (pumping->pumpState == PUMP_ON) {
                log_message(LOG_LEVEL_INFO, "[MODE] Pump is ON, turning OFF before switching to MANUAL mode."); 
                pumping->turn_pump_off(pumping); 
            }
            cfg->mode = MODE_MANUAL;
            log_message(LOG_LEVEL_INFO, "[MODE] Switched to MANUAL"); 
        } else { 
            cfg->mode = MODE_AUTO;
            log_message(LOG_LEVEL_INFO, "[MODE] Switched to AUTO"); 
        }
    } else if (input == 'm') {
        if (cfg->mode == MODE_MANUAL) {
            if (pumping->pumpState == PUMP_OFF) {
                log_message(LOG_LEVEL_INFO, "[MANUAL MODE] Activating manual watering for %d seconds.", cfg->manual_watering_duration_sec); 
                pumping->turn_pump_on(pumping); 
                pumping->manual_watering_start_time = time(NULL); 
            } else {
                log_message(LOG_LEVEL_INFO, "[MANUAL MODE] Pump is already ON (manual or auto). Cannot activate manual watering."); 
            }
        } else {
            log_message(LOG_LEVEL_WARN, "[MANUAL MODE] Manual watering only available in MANUAL mode. Current mode: AUTO"); 
        }
    } else {
        log_message(LOG_LEVEL_INFO, "Invalid button input '%c'. Please press 'a' or 'm'.", input); 
    }
}