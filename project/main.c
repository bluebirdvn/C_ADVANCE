#include "config.h"
#include "sensor.h"
#include "subscriber.h"
#include "sensor_factory.h"
#include "moisture_sensor.h"
#include "modeswitch.h"
#include "pumping.h"
#include "button.h"
#include "publisher.h"
#include "log.h" 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h> 



int main() {
    system_config_init(); 
    SystemConfig *system = system_config_get();

    Pumping pumping;
    pump_init(&pumping);

    Sensors *moistureSensor = CreateSensor(MOISTURE_SENSOR);
    if (moistureSensor == NULL) {
        log_message(LOG_LEVEL_ERROR, "Failed to create moisture sensor. Exiting application."); 
        return 1; 
    }
    
    moistureSensor->init(moistureSensor); 

    moistureSensor->base.subscribeToPub(&moistureSensor->base, &pumping.base);
    log_message(LOG_LEVEL_INFO, "Pumping subscribed to Moisture Sensor."); 

    buttons_init();
    
    log_message(LOG_LEVEL_INFO, "--- Smart Plant Watering System Started ---"); 
    
    while (1) {
        moistureSensor->readData(moistureSensor); 

        button_check_events(&pumping);

        if (system->mode == MODE_AUTO) { 
            autoModeImplement(&pumping, system); 
        } else if (system->mode == MODE_MANUAL) {
            manualModeImplement(&pumping, system); 
        }

        pumping.update_led_status(&pumping); 

        sleep(1);
    }

    free(moistureSensor); 

    return 0;
}