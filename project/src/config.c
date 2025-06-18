#include "config.h"

static SystemConfig config_instance;
SensorData current_sensor_data;
SystemConfig* system_config_get() {
    return &config_instance;
}


void system_config_init() {
    config_instance.min_moisture = 30;
    config_instance.max_moisture = 70;
    config_instance.max_watering_time_sec = 60; // Thời gian tưới tối đa cho một lần tự động
    config_instance.sensor_check_interval_sec = 5; // Chu kỳ kiểm tra cảm biến
    config_instance.manual_watering_duration_sec = 10; // Thời gian tưới thủ công
    config_instance.mode = MODE_AUTO;
}
