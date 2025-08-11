// File: model/model.c

#include "model.h"
#include <unistd.h> // For sleep()
#include <string.h> // For memcpy
#include <stdio.h>

// Singleton instance for the model
static System_monitor g_monitor_instance;
static int g_is_initialized = 0;

// Forward declaration for the thread function
static void* update_metrics_thread_func(void *arg);
static void check_for_alerts(System_monitor *monitor);

System_monitor *system_monitor_init(void) {
    if (g_is_initialized) {
        return &g_monitor_instance;
    }

    // --- Step 1: Initialize all sub-modules using their singletons ---
    log_message(LOG_INFO, "Initializing System Monitor...");
    
    // Initialize logger first
    logger_init("monitor.log", LOG_DEBUG);

    g_monitor_instance.cpu_manager = CPU_manager_init();
    g_monitor_instance.memory_info = Memory_info_init();
    g_monitor_instance.network_info = network_info_instance();
    g_monitor_instance.storage_info = storage_info_instance();
    g_monitor_instance.system_info = system_info_instance();

    // --- Step 2: Set default thresholds ---
    g_monitor_instance.thresholds.cpu_usage_percent = 90.0;
    g_monitor_instance.thresholds.memory_usage_percent = 85.0;
    g_monitor_instance.thresholds.storage_usage_percent = 90.0;
    log_message(LOG_INFO, "Default thresholds set: CPU > %.1f%%, Memory > %.1f%%, Storage > %0.1f%%",
                g_monitor_instance.thresholds.cpu_usage_percent,
                g_monitor_instance.thresholds.memory_usage_percent,
                g_monitor_instance.thresholds.storage_usage_percent);


    // --- Step 3: Initialize mutex and thread ---
    if (pthread_mutex_init(&g_monitor_instance.data_mutex, NULL) != 0) {
        log_message(LOG_CRITICAL, "Failed to initialize mutex. Exiting.");
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    g_monitor_instance.is_running = 1;
    if (pthread_create(&g_monitor_instance.update_thread, NULL, update_metrics_thread_func, &g_monitor_instance) != 0) {
        log_message(LOG_CRITICAL, "Failed to create update thread. Exiting.");
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    log_message(LOG_INFO, "System Monitor initialized successfully. Update thread started.");
    g_is_initialized = 1;
    return &g_monitor_instance;
}

void system_monitor_destroy(System_monitor *monitor) {
    if (!monitor || !g_is_initialized) return;

    log_message(LOG_INFO, "Shutting down System Monitor...");
    monitor->is_running = 0; // Signal the thread to stop

    // Wait for the thread to finish
    pthread_join(monitor->update_thread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&monitor->data_mutex);
    
    // Clean up logger
    logger_clean();
    
    g_is_initialized = 0;
    log_message(LOG_INFO, "System Monitor shut down complete.");
}

void system_monitor_get_data_snapshot(System_monitor *monitor, System_monitor *destination) {
    pthread_mutex_lock(&monitor->data_mutex);
    // Copy the entire data structure safely
    memcpy(destination, monitor, sizeof(System_monitor));
    pthread_mutex_unlock(&monitor->data_mutex);
}

void system_monitor_set_thresholds(System_monitor *monitor, Monitor_thresholds new_thresholds) {
    pthread_mutex_lock(&monitor->data_mutex);
    monitor->thresholds = new_thresholds;
    pthread_mutex_unlock(&monitor->data_mutex);
    log_message(LOG_NOTICE, "Thresholds updated.");
}


/**
 * @brief This is the core function for the background thread.
 * It periodically updates all system metrics.
 */
static void* update_metrics_thread_func(void *arg) {
    System_monitor *monitor = (System_monitor*)arg;

    // Initial sleep to allow system to stabilize before first reading.
    sleep(1);

    while (monitor->is_running) {
        pthread_mutex_lock(&monitor->data_mutex);

        // --- Update all metrics ---
        // CPU
        monitor->cpu_manager->cpu_get_times(monitor->cpu_manager);
        monitor->cpu_manager->calculate_cpu_usage(monitor->cpu_manager);
        monitor->cpu_manager->get_cpu_frequency(monitor->cpu_manager);
        // NOTE: The temperature simulation writes to a file. In a real scenario, this might not be needed.
        monitor->cpu_manager->simulation_temperature(); 
        monitor->cpu_manager->get_cpu_temperature(monitor->cpu_manager);

        // Memory
        monitor->memory_info->get_memory_info(monitor->memory_info);

        // System
        monitor->system_info->get_up_time(monitor->system_info);
        monitor->system_info->load_average(monitor->system_info);

        // Network (Use the standalone functions as defined in your API)
        // These are fast reads
        get_tx_rx_info(monitor->network_info); 
        get_packet_info(monitor->network_info);
        get_connection_info(monitor->network_info);

        // NOTE: The functions below are SLOW and BLOCKING.
        // In a real application, you might run them less frequently.
        // For this example, we run them in the background thread.
        get_network_speed(monitor->network_info); // This contains a sleep(1)

        // Storage
        get_storage_info(monitor->storage_info);

        // --- Check for alerts ---
        check_for_alerts(monitor);

        pthread_mutex_unlock(&monitor->data_mutex);

        // Wait for the next update cycle.
        // The total cycle time will be this sleep + time taken by functions above.
        // Since get_network_speed() already sleeps for 1s, we can make this short.
        sleep(1);
    }
    log_message(LOG_INFO, "Update thread is shutting down.");
    return NULL;
}


/**
 * @brief Checks current data against thresholds and logs alerts.
 */
static void check_for_alerts(System_monitor *monitor) {
    // This function is called inside a mutex lock, so data is consistent.
    
    // Check CPU Usage
    if (monitor->cpu_manager->usage > monitor->thresholds.cpu_usage_percent) {
        log_message(LOG_WARNING, "High CPU Usage Alert: %.2f%% (Threshold: >%.1f%%)",
                    monitor->cpu_manager->usage,
                    monitor->thresholds.cpu_usage_percent);
    }

    // Check Memory Usage
    if (monitor->memory_info->total_memory > 0) {
        double mem_usage_percent = 100.0 * monitor->memory_info->used_memory / monitor->memory_info->total_memory;
        if (mem_usage_percent > monitor->thresholds.memory_usage_percent) {
            log_message(LOG_WARNING, "High Memory Usage Alert: %.2f%% (Threshold: >%.1f%%)",
                        mem_usage_percent,
                        monitor->thresholds.memory_usage_percent);
        }
    }

    if (monitor->storage_info.capacity.total_bytes > 0) {
        double storage_usage_percent = 100 * (1.0)(monitor->storage_info.capacity.used_bytes)/(monitor->storage_info.capacity.total_bytes);

        if (storage_usage_percent > monitor->thresholds.storage_usage_percent) {
            log_message(LOG_WARNING, "High Storage Usage Alert: % .2f%% (Threshold: >%.1f%%)",
                        storage_usage_percent,
                        monitor->thresholds.storage_usage_percent);
        }
    }
}