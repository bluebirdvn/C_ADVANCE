#include "memory.h"

void get_memory_info(Memory_info *memory_info)
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "MemTotal:", 9) == 0) {
            sscanf(buffer, "MemTotal" ": %lu kB", &memory_info->total_memory);
        }
        else if (strncmp(buffer, "MemFree:", 8) == 0) {
            sscanf(buffer, "MemFree" ": %lu kB", &memory_info->free_memory);
        }
        else if (strncmp(buffer, "MemAvailable:", 13) == 0) {
            uint64_t available_memory;
            sscanf(buffer, "MemAvailable" ": %lu kB", &available_memory);
            memory_info->used_memory = memory_info->total_memory - available_memory;
        }
        else if (strncmp(buffer, "SwapTotal:", 10) == 0) {
            sscanf(buffer, "SwapTotal" ": %lu kB", &memory_info->swap_total);
        }
        else if (strncmp(buffer, "SwapFree:", 9) == 0) {
            sscanf(buffer, "SwapFree" ": %lu kB", &memory_info->swap_free);
            memory_info->swap_used = memory_info->swap_total - memory_info->swap_free;
        }
        else if (strncmp(buffer, "Cached:", 7) == 0) {
            sscanf(buffer, "Cached" ": %lu kB", &memory_info->cached_memory);
        }
        else if (strncmp(buffer, "Buffers:", 8) == 0) {
            sscanf(buffer, "Buffers" ": %lu kB", &memory_info->buffers_memory);
        }
    }
    fclose(fp);

}


void extract_process_info(void) 
{
    FILE *f_open, *f_write;
    f_open = popen("ps -eo pid,comm,%mem --sort=-%mem | head -n 6", "r");
    if (!f_open) {
        perror("Failed to run command");
        return;
    }

    f_write = fopen("/home/shunkun/top_ram.txt", "w");
    if (!f_write) {
        perror("Failed to open top_ram.txt");
        pclose(f_open);
        return;
    }
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f_open)) {
        fprintf(f_write, "%s", buffer);
    }

    pclose(f_open);
    fclose(f_write);
    
}

void get_processes_by_ram(Cpu_process_info *processes, int num_processes) 
{
    FILE *fp = fopen("/home/shunkun/top_ram.txt", "r");
    if (!fp) {
        perror("Failed to open top_ram.txt");
        return;
    }

    char buffer[256];
    int count = 0;
    fgets(buffer, sizeof(buffer), fp);
    while (fgets(buffer, sizeof(buffer), fp) && num_processes > 0) {
        sscanf(buffer, "%lu %s %lf", &processes[count].pid, processes[count].name, &processes[count].memory_usage);
        count++;
        num_processes--;
    }

    fclose(fp);
}

int main(void)
{
    int num_processes = 5;
    Cpu_process_info processes[num_processes];
    
    Memory_info memory_info;
    get_memory_info(&memory_info);
    printf("Total Memory: %lu kB\n", memory_info.total_memory);
    printf("Free Memory: %lu kB\n", memory_info.free_memory);
    printf("Used Memory: %lu kB\n", memory_info.used_memory);
    printf("Swap Total: %lu kB\n", memory_info.swap_total);
    printf("Swap Used: %lu kB\n", memory_info.swap_used);
    printf("Swap Free: %lu kB\n", memory_info.swap_free);
    printf("Cached Memory: %lu kB\n", memory_info.cached_memory);
    printf("Buffers Memory: %lu kB\n", memory_info.buffers_memory);

    extract_process_info();
    get_processes_by_ram(processes, num_processes);
    for (int i = 0; i < num_processes; i++) {
        printf("PID: %lu, Name: %s, Memory Usage: %lf %%\n",
               processes[i].pid, processes[i].name, processes[i].memory_usage); 
        }
    return 0;
}