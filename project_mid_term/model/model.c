#include "model.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static SystemMonitor g_monitor_instance;
static int g_is_initialized = 0;

static void* update_metrics_thread_func(void *arg);
static void check_for_alerts(SystemMonitor *monitor);

SystemMonitor *system_monitor_init(void) {
    if (g_is_initialized) {
        return &g_monitor_instance;
    }
    
    memset(&g_monitor_instance, 0, sizeof(SystemMonitor));

    logger_init("monitor.log", LOG_DEBUG);
    log_message(LOG_INFO, "Initializing System Monitor...");

    // --- Step 1: Initialize all sub-modules ---
    g_monitor_instance.cpu_manager = cpu_manager_create();
    g_monitor_instance.memory_manager = memory_manager_create();
    g_monitor_instance.network_manager = network_manager_create();
    g_monitor_instance.storage_manager = storage_manager_create();
    g_monitor_instance.system_manager = system_manager_create();

    if (!g_monitor_instance.cpu_manager || !g_monitor_instance.memory_manager || !g_monitor_instance.network_manager ||
        !g_monitor_instance.storage_manager || !g_monitor_instance.system_manager) {
        log_message(LOG_CRITICAL, "Failed to create one or more managers. Aborting.");
        // We should destroy any that were successfully created
        system_monitor_destroy(&g_monitor_instance);
        return NULL;
    }
    
    // --- Step 2: Set default thresholds ---
    g_monitor_instance.thresholds.cpu_usage_percent = 90.0;
    g_monitor_instance.thresholds.memory_usage_percent = 85.0;
    g_monitor_instance.thresholds.storage_usage_percent = 90.0;
    log_message(LOG_INFO, "Default thresholds set: CPU > %.1f%%, Memory > %.1f%%, Storage > %.1f%%",
                g_monitor_instance.thresholds.cpu_usage_percent,
                g_monitor_instance.thresholds.memory_usage_percent,
                g_monitor_instance.thresholds.storage_usage_percent);

    // --- Step 3: Initialize mutex and thread ---
    if (pthread_mutex_init(&g_monitor_instance.data_mutex, NULL) != 0) {
        log_message(LOG_CRITICAL, "Failed to initialize mutex. Exiting.");
        exit(EXIT_FAILURE);
    }

    g_monitor_instance.is_running = 1;
    if (pthread_create(&g_monitor_instance.update_thread, NULL, update_metrics_thread_func, &g_monitor_instance) != 0) {
        log_message(LOG_CRITICAL, "Failed to create update thread. Exiting.");
        g_monitor_instance.is_running = 0;
        system_monitor_destroy(&g_monitor_instance);
        exit(EXIT_FAILURE);
    }
    
    log_message(LOG_INFO, "System Monitor initialized successfully. Update thread started.");
    g_is_initialized = 1;
    return &g_monitor_instance;
}

void system_monitor_destroy(SystemMonitor *monitor) {
    if (!monitor) return;

    if (monitor->is_running) {
        log_message(LOG_INFO, "Shutting down System Monitor...");
        monitor->is_running = 0;
        pthread_join(monitor->update_thread, NULL);
        log_message(LOG_INFO, "Update thread stopped.");
    }
    
    pthread_mutex_destroy(&monitor->data_mutex);

    // Destroy all managers
    cpu_manager_destroy();
    memory_manager_destroy();
    network_manager_destroy();
    storage_manager_destroy();
    system_manager_destroy();
    
    log_message(LOG_INFO, "All managers destroyed.");
    logger_clean();
    g_is_initialized = 0;
}

void system_monitor_get_data_snapshot(SystemMonitor *monitor, SystemMonitor *destination) {
    pthread_mutex_lock(&monitor->data_mutex);
    memcpy(destination, monitor, sizeof(SystemMonitor));
    pthread_mutex_unlock(&monitor->data_mutex);
}

void system_monitor_set_thresholds(SystemMonitor *monitor, Monitor_thresholds new_thresholds) {
    pthread_mutex_lock(&monitor->data_mutex);
    monitor->thresholds = new_thresholds;
    pthread_mutex_unlock(&monitor->data_mutex);
    log_message(LOG_NOTICE, "Thresholds updated.");
}

static void* update_metrics_thread_func(void *arg) {
    SystemMonitor *monitor = (SystemMonitor*)arg;

    sleep(1);

    while (monitor->is_running) {
        pthread_mutex_lock(&monitor->data_mutex);

        // --- Update all metrics ---
        // CPU
        get_usage(monitor->cpu_manager);
        get_frequency(monitor->cpu_manager);
        get_temperature(monitor->cpu_manager);
        get_top_process(monitor->cpu_manager);
        
        // Memory
        memory_update_stats(monitor->memory_manager);
        
        // System
        system_update_uptime(monitor->system_manager);
        system_update_time(monitor->system_manager);
        system_update_load_average(monitor->system_manager);
        system_update_process_count(monitor->system_manager);

        // Network
        get_bytes(monitor->network_manager);
        get_speed(monitor->network_manager);
        get_connections(monitor->network_manager);

        // Storage
        get_storage_info(monitor->storage_manager);
        run_storage_benchmark(monitor->storage_manager);

        // --- Check for alerts ---
        check_for_alerts(monitor);

        pthread_mutex_unlock(&monitor->data_mutex);

        // Wait for the next update cycle.
        sleep(1);
    }
    log_message(LOG_INFO, "Update thread is shutting down.");
    return NULL;
}

static void check_for_alerts(SystemMonitor *monitor) {
    // Check CPU Usage (Overall CPU is at index 0)
    if (monitor->cpu_manager->usage_percent[0] > monitor->thresholds.cpu_usage_percent) {
        log_message(LOG_WARNING, "High CPU Usage Alert: %.2f%% (Threshold: >%.1f%%)",
                    monitor->cpu_manager->usage_percent[0],
                    monitor->thresholds.cpu_usage_percent);
    }

    // Check Memory Usage
    if (monitor->memory_manager->total_kb > 0) {
        double mem_usage_percent = 100.0 * monitor->memory_manager->used_kb / monitor->memory_manager->total_kb;
        if (mem_usage_percent > monitor->thresholds.memory_usage_percent) {
            log_message(LOG_WARNING, "High Memory Usage Alert: %.2f%% (Threshold: >%.1f%%)",
                        mem_usage_percent,
                        monitor->thresholds.memory_usage_percent);
        }
    }

    // Check Storage Usage
    if (monitor->storage_manager->capacity.total_bytes > 0) {
        double storage_usage_percent = 100.0 * monitor->storage_manager->capacity.used_bytes / monitor->storage_manager->capacity.total_bytes;
        if (storage_usage_percent > monitor->thresholds.storage_usage_percent) {
            log_message(LOG_WARNING, "High Storage Usage Alert: %.2f%% (Threshold: >%.1f%%)",
                        storage_usage_percent,
                        monitor->thresholds.storage_usage_percent);
        }
    }
}