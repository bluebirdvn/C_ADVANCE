
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"


#ifndef TOP_N_PROC
#define TOP_N_PROC 5
#endif

struct Cpu_process_info{
    uint64_t pid;
    char     name[256];
    double   memory_usage;
};

struct Memory_info {
    /* RAM */
    uint64_t total_memory;   /* kB */
    uint64_t free_memory;    /* kB */
    uint64_t used_memory;    /* kB */

    /* Swap */
    uint64_t swap_total;     /* kB */
    uint64_t swap_used;      /* kB */
    uint64_t swap_free;      /* kB */

    /* Cache/Buffers */
    uint64_t cached_memory;  /* kB */
    uint64_t buffers_memory; /* kB */

    /* Top processes by RAM */
    Cpu_process_info top[TOP_N_PROC];
    int top_count;

    /* Methods */
    void (*get_memory_info)(struct Memory_info *memory_info);
    void (*extract_process_info)(struct Memory_info *memory_info);
    void (*get_processes_by_ram)(struct Memory_info *memory_info);
};


static void mgr_get_memory_info(Memory_info *memory_info);
static void mgr_extract_process_info(Memory_info *memory_info);
static void mgr_get_processes_by_ram(Memory_info *memory_info);

static Memory_info Memory_info_instance;
static int is_initialized = 0;

static void mgr_get_memory_info(Memory_info *memory_info)
{
    if (!memory_info) return;

    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    char line[256];
    uint64_t mem_total = 0, mem_free = 0, mem_avail = 0;
    uint64_t swap_total = 0, swap_free = 0;
    uint64_t cached = 0, buffers = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %lu kB", &mem_total);
        } else if (strncmp(line, "MemFree:", 8) == 0) {
            sscanf(line, "MemFree: %lu kB", &mem_free);
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %lu kB", &mem_avail);
        } else if (strncmp(line, "SwapTotal:", 10) == 0) {
            sscanf(line, "SwapTotal: %lu kB", &swap_total);
        } else if (strncmp(line, "SwapFree:", 9) == 0) {
            sscanf(line, "SwapFree: %lu kB", &swap_free);
        } else if (strncmp(line, "Cached:", 7) == 0) {
            sscanf(line, "Cached: %lu kB", &cached);
        } else if (strncmp(line, "Buffers:", 8) == 0) {
            sscanf(line, "Buffers: %lu kB", &buffers);
        }
    }
    fclose(fp);

    memory_info->total_memory   = mem_total;
    memory_info->free_memory    = mem_free;
    memory_info->used_memory    = (mem_avail > 0 && mem_total > mem_avail) ? (mem_total - mem_avail) : (mem_total - mem_free);
    memory_info->swap_total     = swap_total;
    memory_info->swap_free      = swap_free;
    memory_info->swap_used      = (swap_total >= swap_free) ? (swap_total - swap_free) : 0;
    memory_info->cached_memory  = cached;
    memory_info->buffers_memory = buffers;
}

static void mgr_extract_process_info(Memory_info *memory_info)
{
    (void)memory_info; 
    
    FILE *pipe = popen("ps -eo pid,comm,%mem --sort=-%mem | head -n 6", "r");
    if (!pipe) {
        perror("Failed to run ps for RAM");
        return;
    }

    FILE *out = fopen("/home/shunkun/top_ram.txt", "w");
    if (!out) {
        perror("Failed to open top_ram.txt");
        pclose(pipe);
        return;
    }

    char buf[256];
    while (fgets(buf, sizeof(buf), pipe)) {
        fputs(buf, out);
    }

    pclose(pipe);
    fclose(out);
}

static void mgr_get_processes_by_ram(Memory_info *memory_info)
{
    if (!memory_info) return;

    FILE *fp = fopen("/home/shunkun/top_ram.txt", "r");
    if (!fp) {
        perror("Failed to open top_ram.txt");
        memory_info->top_count = 0;
        return;
    }

    char line[256];
    int count = 0;

    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        memory_info->top_count = 0;
        return;
    }

    while (count < TOP_N_PROC && fgets(line, sizeof(line), fp)) {
        unsigned long pid = 0;
        double pmem = 0.0;
        char name[256] = {0};

        if (sscanf(line, "%lu %255s %lf", &pid, name, &pmem) == 3) {
            memory_info->top[count].pid = (uint64_t)pid;
            strncpy(memory_info->top[count].name, name, sizeof(memory_info->top[count].name)-1);
            memory_info->top[count].name[sizeof(memory_info->top[count].name)-1] = '\0';
            memory_info->top[count].memory_usage = pmem;
            count++;
        }
    }

    fclose(fp);
    memory_info->top_count = count;
}


Memory_info *Memory_info_init(void)
{
    if (!is_initialized) {
         memset(&Memory_info_instance, 0, sizeof(Memory_info));
        mem->get_memory_info      = mgr_get_memory_info;
        mem->extract_process_info = mgr_extract_process_info;
        mem->get_processes_by_ram = mgr_get_processes_by_ram;

        is_initialized = 1;
    }

    return &Memory_info_instance;
}

/* --------- Ví dụ sử dụng ----------
int main(void)
{
    Memory_info mem;
    memory_manager_init(&mem);

    mem.get_memory_info(&mem);
    printf("Total: %lu kB\n", mem.total_memory);
    printf("Free : %lu kB\n", mem.free_memory);
    printf("Used : %lu kB\n", mem.used_memory);
    printf("Swap Total: %lu kB\n", mem.swap_total);
    printf("Swap Used : %lu kB\n", mem.swap_used);
    printf("Swap Free : %lu kB\n", mem.swap_free);
    printf("Cached: %lu kB\n", mem.cached_memory);
    printf("Buffers: %lu kB\n", mem.buffers_memory);

    mem.extract_process_info(&mem);
    mem.get_processes_by_ram(&mem);
    for (int i = 0; i < mem.top_count; ++i) {
        printf("PID: %lu, Name: %s, MEM: %.2f%%\n",
               mem.top[i].pid, mem.top[i].name, mem.top[i].memory_usage);
    }
    return 0;
}
------------------------------------- */
