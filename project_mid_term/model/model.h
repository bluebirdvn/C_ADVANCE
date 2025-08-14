#ifndef _MODEL_H
#define _MODEL_H

#include <pthread.h> 
#include "cpu.h"
#include "network.h"
#include "storage.h"
#include "system.h"
#include "memory.h"
#include "logger.h"

// Thresholds for alerts
typedef struct {
    double cpu_usage_percent;
    double memory_usage_percent;
    double storage_usage_percent;
} Monitor_thresholds;

// The main Facade struct
typedef struct {
    StorageManager *storage_manager;
    NetworkManager *network_manager;
    MemoryManager  *memory_manager;
    CpuManager     *cpu_manager;
    SystemManager  *system_manager;

    pthread_t update_thread;
    pthread_mutex_t data_mutex;
    volatile int is_running;

    Monitor_thresholds thresholds;

} SystemMonitor;

SystemMonitor *system_monitor_init(void);
void system_monitor_destroy(SystemMonitor *monitor);
void system_monitor_get_data_snapshot(SystemMonitor *monitor, SystemMonitor *destination);
void system_monitor_set_thresholds(SystemMonitor *monitor, Monitor_thresholds new_thresholds);

#endif //_MODEL_H