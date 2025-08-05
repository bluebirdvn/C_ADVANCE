
#include "cpu.h"

#define NUM_CPU 9

void get_cpu_times(uint64_t *user, uint64_t *nice, uint64_t *system, 
                    uint64_t *idle, uint64_t *iowait, uint64_t *irq, uint64_t *softirq, uint64_t *steal)
{
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    int cpu_index = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "cpu", 3) == 0 && isdigit(buffer[3])) {
            if (cpu_index > NUM_CPU) {
                break;
            }
            sscanf(buffer, "cpu%d %ld %ld %ld %ld %ld %ld %ld %ld",
                   &cpu_index, user, nice, system, idle, iowait, irq, softirq, steal);
            cpu_index++;

        }
    }
    fclose(file);
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

void calculate_each_cpu_uage(uint64_t *idle, uint64_t *total, double *usage)
{
    for ( int i =0; i < NUM_CPU; ++i) {
        if (total[i] == 0) {
            usage[i] = 0.0; // Avoid division by zero
        } else {
            usage[i] = 100.0 * (1.0 - ((double)idle[i] / (double)total[i]));
        }
    }
}

void calculate_cpu_usage(float *usage)
{
    uint64_t start[NUM_CPU][8], end[NUM_CPU][8];
    uint64_t idle[NUM_CPU], total[NUM_CPU];
    // get firts CPU times
    for (int i = 0; i < NUM_CPU; ++i) {
    // get firts CPU times
    get_cpu_times(&start[i][0], &start[i][1], &start[i][2],
                  &start[i][3], &start[i][4], &start[i][5], &start[i][6], &start[i][7]);

    sleep(1); // Sleep for 1 second to get the next CPU times

    // get second CPU times
    get_cpu_times(&end[i][0], &end[i][1], &end[i][2],
                  &end[i][3], &end[i][4], &end[i][5], &end[i][6], &end[i][7]);
    }

    // Calculate idle and total CPU times
    for (int i = 0; i < NUM_CPU; ++i) {
        calculate_idle_and_total(&start[i], &end[i], &idle[i], &total[i]);
    }

    calculate_each_cpu_uage(&idle, &total, &usage);
}

int main(void) {
    while (1) 
    {
    float usage[NUM_CPU];
    calculate_cpu_usage(usage);
    for (int i = 0; i < NUM_CPU; ++i) {
        printf("CPU %d Usage: %.2f%%\n", i, usage[i]);
    }
    sleep(2);        
    }

    return 0;
}