// File: main.c
#include "model/model.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h> // For signal handling

// Global pointer to the monitor to be accessible by the signal handler
System_monitor *g_app_monitor = NULL;

void handle_shutdown(int sig) {
    printf("\nCaught signal %d. Shutting down gracefully...\n", sig);
    if (g_app_monitor) {
        system_monitor_destroy(g_app_monitor);
    }
    exit(0);
}

void print_dashboard(System_monitor *snapshot) {
    // Clear screen for a smoother "real-time" feel
    printf("\033[2J\033[H");
    
    printf("===== System Monitor Dashboard (Press Ctrl+C to exit) =====\n");
    
    // --- System Info ---
    printf("\n--- System ---\n");
    printf("Uptime: %d days, %02d:%02d\n",
           snapshot->system_info->uptime.days,
           snapshot->system_info->uptime.hours,
           snapshot->system_info->uptime.minutes);
    printf("Load Average: %.2f (1m), %.2f (5m), %.2f (15m)\n",
           snapshot->system_info->load.load1,
           snapshot->system_info->load.load5,
           snapshot->system_info->load.load15);

    // --- CPU Info ---
    printf("\n--- CPU ---\n");
    printf("Usage: %.2f%%\n", snapshot->cpu_manager->usage);
    printf("Frequency: %.2f MHz\n", snapshot->cpu_manager->frequency);
    printf("Temperature: %.2f°C\n", snapshot->cpu_manager->temperature);
    // Calling the function directly for top processes as it's quick
    snapshot->cpu_manager->get_top_processes_by_cpu();


    // --- Memory Info ---
    printf("\n--- Memory ---\n");
    double used_mem_mb = snapshot->memory_info->used_memory / 1024.0;
    double total_mem_mb = snapshot->memory_info->total_memory / 1024.0;
    double mem_percent = (total_mem_mb > 0) ? (100.0 * used_mem_mb / total_mem_mb) : 0;
    printf("RAM Usage: %.2f / %.2f MB (%.2f%%)\n", used_mem_mb, total_mem_mb, mem_percent);

    // --- Network Info ---
    printf("\n--- Network (Interface: %s) ---\n", network_get_interface());
    printf("Speed Down/Up: %.2f / %.2f kB/s\n",
           snapshot->network_info->speed_info.download_speed_kBps,
           snapshot->network_info->speed_info.upload_speed_kBps);
    printf("Packets RX/TX: %lu / %lu\n",
           snapshot->network_info->packet_info.packet_rx,
           snapshot->network_info->packet_info.packet_tx);
    printf("TCP Connections: %u\n", snapshot->network_info->connection_info.total_tcp_connections);

    // --- Storage Info ---
    printf("\n--- Storage (Mount: %s) ---\n", snapshot->storage_info->mount_path);
    double used_gb = snapshot->storage_info->capacity.used_bytes / (1024.0 * 1024.0 * 1024.0);
    double total_gb = snapshot->storage_info->capacity.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double disk_percent = (total_gb > 0) ? (100.0 * used_gb / total_gb) : 0;
    printf("Disk Usage: %.2f / %.2f GB (%.2f%%)\n", used_gb, total_gb, disk_percent);
    
    fflush(stdout);
}


int main(void) {
    // Graceful shutdown on Ctrl+C
    signal(SIGINT, handle_shutdown);
    
    // Initialize the entire monitoring system
    g_app_monitor = system_monitor_init();

    if (!g_app_monitor) {
        fprintf(stderr, "Failed to initialize the monitor.\n");
        return 1;
    }
    
    printf("Starting dashboard. Data will refresh every 2 seconds.\n");
    sleep(2); // Wait for first data collection

    // This is the main loop for the UI/display
    while (1) {
        System_monitor data_snapshot;
        // Get a thread-safe copy of the latest data
        system_monitor_get_data_snapshot(g_app_monitor, &data_snapshot);

        // Print the dashboard using the copied data
        print_dashboard(&data_snapshot);
        
        // The main loop can now sleep without affecting data collection
        sleep(2);
    }

    // This part is now handled by the signal handler
    // system_monitor_destroy(g_app_monitor);
    return 0;
}