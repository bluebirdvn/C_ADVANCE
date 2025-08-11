// #include "memory.h"

// typedef struct {
//     uint64_t total_memory;
//     uint64_t free_memory;
//     uint64_t used_memory;

//     uint64_t swap_total;
//     uint64_t swap_used;
//     uint64_t swap_free;

//     uint64_t cached_memory;
//     uint64_t buffers_memory;

//     Cpu_process_info cpu_process_info;

//     void (*get_memory_info)(Memory_info *memory_info);
//     void (*extract_process_info)(void);
//     void (*get_processes_by_ram)(Memory_info *memory_info);
//     void (*)
// } Memory_info;

// typedef struct {
//     uint64_t pid;
//     char name[256];
//     double memory_usage;
// } Cpu_process_info;


// void get_memory_info(Memory_info *memory_info)
// {
//     FILE *fp = fopen("/proc/meminfo", "r");
//     if (!fp) {
//         perror("Failed to open /proc/meminfo");
//         return;
//     }

//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), fp)) {
//         if (strncmp(buffer, "MemTotal:", 9) == 0) {
//             sscanf(buffer, "MemTotal" ": %lu kB", &memory_info->total_memory);
//         }
//         else if (strncmp(buffer, "MemFree:", 8) == 0) {
//             sscanf(buffer, "MemFree" ": %lu kB", &memory_info->free_memory);
//         }
//         else if (strncmp(buffer, "MemAvailable:", 13) == 0) {
//             uint64_t available_memory;
//             sscanf(buffer, "MemAvailable" ": %lu kB", &available_memory);
//             memory_info->used_memory = memory_info->total_memory - available_memory;
//         }
//         else if (strncmp(buffer, "SwapTotal:", 10) == 0) {
//             sscanf(buffer, "SwapTotal" ": %lu kB", &memory_info->swap_total);
//         }
//         else if (strncmp(buffer, "SwapFree:", 9) == 0) {
//             sscanf(buffer, "SwapFree" ": %lu kB", &memory_info->swap_free);
//             memory_info->swap_used = memory_info->swap_total - memory_info->swap_free;
//         }
//         else if (strncmp(buffer, "Cached:", 7) == 0) {
//             sscanf(buffer, "Cached" ": %lu kB", &memory_info->cached_memory);
//         }
//         else if (strncmp(buffer, "Buffers:", 8) == 0) {
//             sscanf(buffer, "Buffers" ": %lu kB", &memory_info->buffers_memory);
//         }
//     }
//     fclose(fp);

// }


// void extract_process_info(void) 
// {
//     FILE *f_open, *f_write;
//     f_open = popen("ps -eo pid,comm,%mem --sort=-%mem | head -n 6", "r");
//     if (!f_open) {
//         perror("Failed to run command");
//         return;
//     }

//     f_write = fopen("/home/shunkun/top_ram.txt", "w");
//     if (!f_write) {
//         perror("Failed to open top_ram.txt");
//         pclose(f_open);
//         return;
//     }
    
//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), f_open)) {
//         fprintf(f_write, "%s", buffer);
//     }

//     pclose(f_open);
//     fclose(f_write);
    
// }

// void get_processes_by_ram(Cpu_process_info *processes, int num_processes) 
// {
//     FILE *fp = fopen("/home/shunkun/top_ram.txt", "r");
//     if (!fp) {
//         perror("Failed to open top_ram.txt");
//         return;
//     }

//     char buffer[256];
//     int count = 0;
//     fgets(buffer, sizeof(buffer), fp);
//     while (fgets(buffer, sizeof(buffer), fp) && num_processes > 0) {
//         sscanf(buffer, "%lu %s %lf", &processes[count].pid, processes[count].name, &processes[count].memory_usage);
//         count++;
//         num_processes--;
//     }

//     fclose(fp);
// }

// int main(void)
// {
//     int num_processes = 5;
//     Cpu_process_info processes[num_processes];
    
//     Memory_info memory_info;
//     get_memory_info(&memory_info);
//     printf("Total Memory: %lu kB\n", memory_info.total_memory);
//     printf("Free Memory: %lu kB\n", memory_info.free_memory);
//     printf("Used Memory: %lu kB\n", memory_info.used_memory);
//     printf("Swap Total: %lu kB\n", memory_info.swap_total);
//     printf("Swap Used: %lu kB\n", memory_info.swap_used);
//     printf("Swap Free: %lu kB\n", memory_info.swap_free);
//     printf("Cached Memory: %lu kB\n", memory_info.cached_memory);
//     printf("Buffers Memory: %lu kB\n", memory_info.buffers_memory);

//     extract_process_info();
//     get_processes_by_ram(processes, num_processes);
//     for (int i = 0; i < num_processes; i++) {
//         printf("PID: %lu, Name: %s, Memory Usage: %lf %%\n",
//                processes[i].pid, processes[i].name, processes[i].memory_usage); 
//         }
//     return 0;
// }

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

/* Có thể chuyển mấy typedef này sang memory.h */
#ifndef TOP_N_PROC
#define TOP_N_PROC 5
#endif

typedef struct {
    uint64_t pid;
    char     name[256];
    double   memory_usage; /* %MEM */
} Cpu_process_info;

typedef struct Memory_info {
    /* RAM */
    uint64_t total_memory;   /* kB */
    uint64_t free_memory;    /* kB */
    uint64_t used_memory;    /* kB (dùng MemAvailable để tính thực tế) */

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
} Memory_info;

/* --------- Forward declarations cho function pointers --------- */
static void mgr_get_memory_info(Memory_info *memory_info);
static void mgr_extract_process_info(Memory_info *memory_info);
static void mgr_get_processes_by_ram(Memory_info *memory_info);

/* --------- Implementations --------- */
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
    (void)memory_info; /* Không cần dùng, nhưng giữ signature theo struct */

    /* Lấy top 5 theo %MEM, bỏ header trong lúc parse */
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

    /* Bỏ dòng header */
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        memory_info->top_count = 0;
        return;
    }

    while (count < TOP_N_PROC && fgets(line, sizeof(line), fp)) {
        /* Dạng: " PID COMMAND %MEM" */
        /* name không có khoảng trắng dài – dùng %255s */
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

/* --------- Public init: gán function pointers --------- */
void memory_manager_init(Memory_info *mem)
{
    if (!mem) return;
    memset(mem, 0, sizeof(*mem));

    mem->get_memory_info      = mgr_get_memory_info;
    mem->extract_process_info = mgr_extract_process_info;
    mem->get_processes_by_ram = mgr_get_processes_by_ram;
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
