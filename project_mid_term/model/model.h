

#ifndef _MODEL_H
#define _MODEL_H

#include <pthread.h> 
#include "../cpu/cpu.h"
#include "../network/network.h"
#include "../storage/storage.h"
#include "../system/system.h"
#include "../memory/memory.h"
#include "../logger/logger.h"

// Thresholds for alerts
typedef struct {
    double cpu_usage_percent;
    double memory_usage_percent;
    double storage_usage_percent;
    // Add other thresholds as needed, e.g., temperature
} Monitor_thresholds;

// The main Facade struct
typedef struct {
    // Pointers to singleton instances of sub-modules
    Storage_info *storage_info;
    Network_info *network_info;
    Memory_info  *memory_info;
    CPU_manager  *cpu_manager; // Changed from Cpu_usage to CPU_manager to match your code
    System_info  *system_info;

    // Threading and state
    pthread_t update_thread;
    pthread_mutex_t data_mutex;
    int is_running;

    // Configuration
    Monitor_thresholds thresholds;

} System_monitor;

/**
 * @brief Initializes the system monitor facade.
 * This sets up all sub-modules, configures default thresholds,
 * and starts the background update thread.
 *
 * @return A pointer to the singleton System_monitor instance.
 */
System_monitor *system_monitor_init(void);

/**
 * @brief Cleans up and destroys the system monitor.
 * Stops the background thread and frees resources.
 *
 * @param monitor The monitor instance to destroy.
 */
void system_monitor_destroy(System_monitor *monitor);

/**
 * @brief Thread-safely copies the latest monitoring data.
 *
 * @param monitor The monitor instance.
 * @param destination A pointer to a struct where the data will be copied.
 */
void system_monitor_get_data_snapshot(System_monitor *monitor, System_monitor *destination);

/**
 * @brief Sets new thresholds for monitoring alerts.
 *
 * @param monitor The monitor instance.
 * @param new_thresholds The new threshold values.
 */
void system_monitor_set_thresholds(System_monitor *monitor, Monitor_thresholds new_thresholds);


#endif //_MODEL_H