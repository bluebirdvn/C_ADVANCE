
#include "cpu.h"

void get_cpu_times(uint64_t *user, uint64_t *nice, uint64_t *system, 
                    uint64_t *idle, uint64_t *iowait, uint64_t *irq, uint64_t *softirq, uint64_t *steal) 
{
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    // Parse the CPU line
    sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",
           user, nice, system, idle, iowait, irq, softirq, steal);
}

void calculate_idle_and_total(uint64_t *start, uint64_t *end, uint64_t *idle, uint64_t *total)
{
    uint64_t idle_diff = (end[3] + end[4]) - (start[3] + start[4]);

    uint64_t total_diff = (end[0] + end[1] + end[2] + end[3] + end[4] + 
                           end[5] + end[6] + end[7]) - 
                          (start[0] + start[1] + start[2] + start[3] + start[4] + 
                           start[5] + start[6] + start[7]);

    *idle = idle_diff;
    *total = total_diff;
    
}

int calculate_cpu_usage()
{
    uint64_t start[8], end[8];
    uint64_t idle, total;

    // get firts CPU times
    get_cpu_times(&start[0], &start[1], &start[2],
                  &start[3], &start[4], &start[5], &start[6], &start[7]);

    sleep(1); // Sleep for 1 second to get the next CPU times

    // get second CPU times
    get_cpu_times(&end[0], &end[1], &end[2],
                  &end[3], &end[4], &end[5], &end[6], &end[7]);
    
    // Calculate idle and total CPU times
    calculate_idle_and_total(start, end, &idle, &total);

    if (total == 0) {
        return 0; // Avoid division by zero
    }
    // Calculate CPU usage percentage
    if (idle == 0) {
        return 100; // If idle is zero, CPU is fully utilized
    }
    if (idle == total) {
        return 0; // If idle equals total, CPU is fully idle
    }

    double cpu_usage = 1.0 - ((double)idle /(total));
    return cpu_usage * 100; // Return CPU usage as a percentage
}

int main(void) {
    while (1) 
    {
    double usage = calculate_cpu_usage();
    printf("CPU Usage: %.2f%%\n", usage);
    sleep(2);        
    }

    return 0;
}