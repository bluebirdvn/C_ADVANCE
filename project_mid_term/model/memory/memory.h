
#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

typedef struct {
    uint64_t total_memory;
    uint64_t free_memory;
    uint64_t used_memory;

    uint64_t swap_total;
    uint64_t swap_used;
    uint64_t swap_free;

    uint64_t cached_memory;
    uint64_t buffers_memory;

    Cpu_process_info cpu_process_info;
} Memory_info;

typedef struct {
    uint64_t pid;
    char name[256];
    double memory_usage;
} Cpu_process_info;


/*
*@function: get_memory_info
*@descripton: get the memory information from /proc/meminfo and /proc/swaps
*@param: memory_info: a pointer to a Memory_info struct to store the memory infomation
*/
void get_memory_info(Memory_info *memory_info);

/*
*@function: extract_process_info;
*@description: extract and save top processes by RAM usage to a file
*/
void extract_process_info(void);

/*
*@function: get_processes_by_cpu
*@description: get the top processes by CPU usage
*@param: processes: an array of Cpu_process_info to store the processes information
*@param: num_processes: the number of processes to retrieve
*/
void get_processes_by_ram(Cpu_process_info *processes, int num_processes);


#endif // _MEMORY_H