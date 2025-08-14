#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROC_MEMINFO_PATH "/proc/meminfo"

static MemoryManager *g_memory_manager_instance = NULL;

// --- Internal Function Implementations ---
static void internal_update_stats(void);
static void internal_update_top_processes(void);


void memory_manager_destroy(void) {
    if (g_memory_manager_instance) {
        free(g_memory_manager_instance);
        g_memory_manager_instance = NULL;
    }
}

MemoryManager *memory_manager_create(void) {
    if (g_memory_manager_instance == NULL) {
        g_memory_manager_instance = malloc(sizeof(MemoryManager));
        if (!g_memory_manager_instance) {
            perror("Failed to allocate memory for MemoryManager");
            return NULL;
        }
        memset(g_memory_manager_instance, 0, sizeof(MemoryManager));

        // Assign internal methods
        g_memory_manager_instance->update_stats = internal_update_stats;
        g_memory_manager_instance->update_top_processes = internal_update_top_processes;
    }
    return g_memory_manager_instance;
}


static void internal_update_stats(void) {
    if (g_memory_manager_instance == NULL) return;

    FILE *fp = fopen(PROC_MEMINFO_PATH, "r");
    if (!fp) {
        perror("Failed to open " PROC_MEMINFO_PATH);
        return;
    }

    char line[256];
    uint64_t mem_avail = 0;

    // Reset values before reading
    g_memory_manager_instance->total_kb = 0;
    g_memory_manager_instance->free_kb = 0;
    g_memory_manager_instance->swap_total_kb = 0;
    g_memory_manager_instance->swap_free_kb = 0;
    g_memory_manager_instance->cached_kb = 0;
    g_memory_manager_instance->buffers_kb = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) sscanf(line, "MemTotal: %lu kB", &g_memory_manager_instance->total_kb);
        else if (strncmp(line, "MemFree:", 8) == 0) sscanf(line, "MemFree: %lu kB", &g_memory_manager_instance->free_kb);
        else if (strncmp(line, "MemAvailable:", 13) == 0) sscanf(line, "MemAvailable: %lu kB", &mem_avail);
        else if (strncmp(line, "SwapTotal:", 10) == 0) sscanf(line, "SwapTotal: %lu kB", &g_memory_manager_instance->swap_total_kb);
        else if (strncmp(line, "SwapFree:", 9) == 0) sscanf(line, "SwapFree: %lu kB", &g_memory_manager_instance->swap_free_kb);
        else if (strncmp(line, "Cached:", 7) == 0) sscanf(line, "Cached: %lu kB", &g_memory_manager_instance->cached_kb);
        else if (strncmp(line, "Buffers:", 8) == 0) sscanf(line, "Buffers: %lu kB", &g_memory_manager_instance->buffers_kb);
    }
    fclose(fp);

    g_memory_manager_instance->used_kb = (mem_avail > 0) 
        ? (g_memory_manager_instance->total_kb - mem_avail) 
        : (g_memory_manager_instance->total_kb - g_memory_manager_instance->free_kb);
    g_memory_manager_instance->swap_used_kb = g_memory_manager_instance->swap_total_kb - g_memory_manager_instance->swap_free_kb;
}

static void internal_update_top_processes(void) {
    if (g_memory_manager_instance == NULL) return;

    // This command is inefficient but simple. For a real product, parsing /proc is better.
    char command[256];
    snprintf(command, sizeof(command), "ps -eo pid,comm,%%mem --sort=-%%mem | head -n %d", MEMORY_TOP_PROCESSES + 1);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("Failed to run ps command");
        g_memory_manager_instance->top_process_count = 0;
        return;
    }

    char line[256];
    // Skip header line
    if (!fgets(line, sizeof(line), pipe)) {
        pclose(pipe);
        g_memory_manager_instance->top_process_count = 0;
        return;
    }

    int count = 0;
    while (count < MEMORY_TOP_PROCESSES && fgets(line, sizeof(line), pipe)) {
        Process_mem_info *proc = &g_memory_manager_instance->top_processes[count];
        if (sscanf(line, "%lu %63s %lf", &proc->pid, proc->name, &proc->usage_percent) == 3) {
            count++;
        }
    }
    pclose(pipe);
    g_memory_manager_instance->top_process_count = count;
}


// --- Public API Implementations ---

void memory_update_stats(MemoryManager *manager) {
    if (manager == NULL) return;
    manager->update_stats();
}

void memory_update_top_processes(MemoryManager *manager) {
    if (manager == NULL) return;
    manager->update_top_processes();    
}