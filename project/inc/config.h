#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <stdint.h>


typedef enum {
    MODE_AUTO,
    MODE_MANUAL
} SYSTEM_MODE;

typedef enum {
    PUMP_OFF,
    PUMP_ON
} PUMP_STATE;

typedef enum {
    LED_NORMAL,
    LED_WATERING,
    LED_LOW_MOISTURE_ALERT,
    LED_ERROR
} LED_STATE;

typedef enum {
    CODE_SUCCESS = 0,
    CODE_ERROR
}RetCode_t;

typedef struct {
    uint8_t moisture_percent;
    int8_t temperature_c;
} SensorData;

typedef struct {
    uint8_t min_moisture;
    uint8_t max_moisture;
    uint16_t max_watering_time_sec;
    uint16_t sensor_check_interval_sec;
    uint16_t manual_watering_duration_sec;
    SYSTEM_MODE mode;
} SystemConfig;

extern SensorData current_sensor_data;

SystemConfig* system_config_get();
void system_config_init();


#endif