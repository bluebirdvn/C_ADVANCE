#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "model.h"

// --- View Functions ---
void display_summary(SystemMonitor *data);
void display_cpu_details(SystemMonitor *data);
void display_memory_details(SystemMonitor *data);
void display_network_details(SystemMonitor *data);
void display_storage_details(SystemMonitor *data);
void display_system_details(SystemMonitor *data);

typedef enum {
    VIEW_SUMMARY,
    VIEW_CPU,
    VIEW_MEMORY,
    VIEW_NETWORK,
    VIEW_STORAGE,
    VIEW_SYSTEM
} current_view_t;

int main(void) {
    // Initialize the monitoring system
    SystemMonitor *monitor = system_monitor_init();
    if (!monitor) {
        fprintf(stderr, "Failed to initialize the system monitor.\n");
        return 1;
    }

    SystemMonitor data_snapshot;
    current_view_t view = VIEW_SUMMARY;
    int loop = 1;

    while (loop) {
        // Get a thread-safe copy of the latest data
        system_monitor_get_data_snapshot(monitor, &data_snapshot);

        // Clear screen
        printf("\033[2J\033[H");
        printf("===== Linux System Monitor =====\n");
        printf("Time: %s\n\n", data_snapshot.system_manager->clock.iso_datetime);

        switch (view) {
            case VIEW_SUMMARY:  display_summary(&data_snapshot); break;
            case VIEW_CPU:      display_cpu_details(&data_snapshot); break;
            case VIEW_MEMORY:   display_memory_details(&data_snapshot); break;
            case VIEW_NETWORK:  display_network_details(&data_snapshot); break;
            case VIEW_STORAGE:  display_storage_details(&data_snapshot); break;
            case VIEW_SYSTEM:   display_system_details(&data_snapshot); break;
        }

        printf("\n[ (m)ain | (c)pu | (e)memory | (n)etwork | (s)torage | s(y)stem | (q)uit ]\n");
        printf("Enter your choice: ");
        
        // Simple blocking input
        char choice = getchar();
        while(getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 'm': view = VIEW_SUMMARY; break;
            case 'c': view = VIEW_CPU; break;
            case 'e': view = VIEW_MEMORY; break;
            case 'n': view = VIEW_NETWORK; break;
            case 's': view = VIEW_STORAGE; break;
            case 'y': view = VIEW_SYSTEM; break;
            case 'q': loop = 0; break;
        }
        // No sleep needed here, getchar() blocks
    }

    // Clean up
    printf("Shutting down...\n");
    system_monitor_destroy(monitor);
    printf("Done.\n");

    return 0;
}


void display_summary(SystemMonitor *data) {
    printf("--- System Summary ---\n");
    // CPU
    printf("CPU Usage: %5.1f%%   | Temp: %.1f°C   | Freq: %.0f MHz\n", 
        data->cpu_manager->usage_percent[0], 
        data->cpu_manager->temperature_celsius, 
        data->cpu_manager->frequency_mhz);
    // Memory
    double mem_used_gb = data->memory_manager->used_kb / 1024.0 / 1024.0;
    double mem_total_gb = data->memory_manager->total_kb / 1024.0 / 1024.0;
    printf("Mem Usage: %5.1f%%   | %5.2f / %.2f GB\n", 
        100.0 * mem_used_gb / mem_total_gb,
        mem_used_gb, 
        mem_total_gb);
    // Storage
    double disk_used_gb = data->storage_manager->capacity.used_bytes / 1e9;
    double disk_total_gb = data->storage_manager->capacity.total_bytes / 1e9;
    printf("Disk Usage: %4.1f%%  | %5.1f / %.1f GB (%s)\n",
        100.0 * disk_used_gb / disk_total_gb,
        disk_used_gb,
        disk_total_gb,
        data->storage_manager->mount_path);
    // Network
    printf("Network RX: %7.2f kbps | TX: %7.2f kbps (%s)\n",
        data->network_manager->rx_speed_kbps,
        data->network_manager->tx_speed_kbps,
        data->network_manager->interface_name);
}

void display_cpu_details(SystemMonitor *data) {
    CpuManager *cpu = data->cpu_manager;
    printf("--- CPU Details ---\n");
    printf("Overall Usage : %5.1f%%\n", cpu->usage_percent[0]);
    printf("Frequency     : %.2f MHz\n", cpu->frequency_mhz);
    printf("Temperature   : %.1f °C (Simulated)\n", cpu->temperature_celsius);
    printf("Top Process   : %s\n", cpu->top_process);
    printf("\nPer-Core Usage:\n");
    for (int i = 1; i < cpu->core_count; i++) {
        printf("  Core %-2d : %5.1f%%\n", i - 1, cpu->usage_percent[i]);
    }
}

void display_memory_details(SystemMonitor *data) {
    MemoryManager *mem = data->memory_manager;
    printf("--- Memory Details ---\n");
    printf("RAM Total     : %lu MB\n", mem->total_kb / 1024);
    printf("RAM Used      : %lu MB (%.1f%%)\n", mem->used_kb / 1024, 100.0 * mem->used_kb/mem->total_kb);
    printf("RAM Free      : %lu MB\n", mem->free_kb / 1024);
    printf("Buffers       : %lu MB\n", mem->buffers_kb / 1024);
    printf("Cached        : %lu MB\n", mem->cached_kb / 1024);
    printf("\nSwap Total    : %lu MB\n", mem->swap_total_kb / 1024);
    printf("Swap Used     : %lu MB (%.1f%%)\n", mem->swap_used_kb / 1024, (mem->swap_total_kb > 0) ? (100.0 * mem->swap_used_kb / mem->swap_total_kb) : 0);
    printf("Swap Free     : %lu MB\n", mem->swap_free_kb / 1024);
}

void display_network_details(SystemMonitor *data) {
    NetworkManager *net = data->network_manager;
    printf("--- Network Details for Interface: %s ---\n", net->interface_name);
    printf("Status        : %s\n", net->link_up ? "UP" : "DOWN");
    printf("IPv4 Address  : %s\n", net->ipv4_count > 0 ? net->ipv4_addrs[0] : "N/A");
    printf("IPv6 Address  : %s\n", net->ipv6_count > 0 ? net->ipv6_addrs[0] : "N/A");
    printf("\n--- Speed ---\n");
    printf("RX Speed      : %.2f kbps (%.2f MB/s)\n", net->rx_speed_kbps, net->rx_speed_kbps / 8 / 1024);
    printf("TX Speed      : %.2f kbps (%.2f MB/s)\n", net->tx_speed_kbps, net->tx_speed_kbps / 8 / 1024);
    printf("\n--- Data Transfer ---\n");
    printf("Received      : %.2f GB\n", (double)net->rx_bytes / 1e9);
    printf("Transmitted   : %.2f GB\n", (double)net->tx_bytes / 1e9);
    printf("\n--- Connections ---\n");
    printf("TCP Total     : %d\n", net->tcp_connections_total);
    printf("TCP Established: %d\n", net->tcp_connections_established);
}

void display_storage_details(SystemMonitor *data) {
    StorageManager *sto = data->storage_manager;
    printf("--- Storage Details for Mount: %s ---\n", sto->mount_path);
    printf("Device        : /dev/%s\n", sto->device_name);
    printf("\n--- Capacity ---\n");
    printf("Total size    : %.2f GB\n", (double)sto->capacity.total_bytes / 1e9);
    printf("Used space    : %.2f GB (%.1f%%)\n", (double)sto->capacity.used_bytes / 1e9, 100.0 * sto->capacity.used_bytes / sto->capacity.total_bytes);
    printf("Free space    : %.2f GB\n", (double)sto->capacity.free_bytes / 1e9);
    printf("\n--- I/O (since boot) ---\n");
    printf("Data Read     : %.2f GB\n", (double)sto->sectors.bytes_read / 1e9);
    printf("Data Written  : %.2f GB\n", (double)sto->sectors.bytes_written / 1e9);
}

void display_system_details(SystemMonitor *data) {
    SystemManager *sys = data->system_manager;
    printf("--- System Information ---\n");
    printf("OS / Kernel   : %s %s\n", sys->kernel.sysname, sys->kernel.kernel_release);
    printf("Architecture  : %s\n", sys->kernel.machine);
    printf("Uptime        : %d days, %02d:%02d\n", sys->uptime.days, sys->uptime.hours, sys->uptime.minutes);
    printf("Load Average  : %.2f (1m), %.2f (5m), %.2f (15m)\n", sys->load.load1, sys->load.load5, sys->load.load15);
    printf("Total Procs   : %u\n", sys->proc.process_count);
}