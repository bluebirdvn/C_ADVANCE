
#include "cpu.h"



int count_num_cpus(void) 
{
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu", 3) == 0 && isdigit(line[3])) {
            count++;
        }
    }

    fclose(file);
    return count;
}

void get_cpu_times(uint64_t cpu_times[NUM_CPU][8])
{
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    int cpu_index = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "cpu", 3) == 0) {
            if (buffer[3] == ' ' || buffer[3] == '\t') {
                // This is the total CPU line
                sscanf(buffer, "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
                       &cpu_times[0][0], &cpu_times[0][1],
                       &cpu_times[0][2], &cpu_times[0][3],
                       &cpu_times[0][4], &cpu_times[0][5],
                       &cpu_times[0][6], &cpu_times[0][7]);
            } else {
                // This is a per-CPU line
                sscanf(buffer, "cpu%d %lu %lu %lu %lu %lu %lu %lu %lu",
                       &cpu_index,
                       &cpu_times[cpu_index][0], &cpu_times[cpu_index][1],
                       &cpu_times[cpu_index][2], &cpu_times[cpu_index][3],
                       &cpu_times[cpu_index][4], &cpu_times[cpu_index][5],
                       &cpu_times[cpu_index][6], &cpu_times[cpu_index][7]);
                cpu_index++;
            }
    
        } else break;
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

void calculate_cpu_usage(double *usage)
{
    uint64_t start[NUM_CPU][8], end[NUM_CPU][8];
    uint64_t idle[NUM_CPU], total[NUM_CPU];
    // get first CPU times
    get_cpu_times(start);

    sleep(1); // Sleep for 1 second to get the next CPU times

    // get second CPU times
    get_cpu_times(end);


    // Calculate idle and total CPU times
    for (int i = 0; i < NUM_CPU; ++i) {
        calculate_idle_and_total(start[i], end[i], &idle[i], &total[i]);
    }

    calculate_each_cpu_uage(idle, total, usage);
}

int main(void) {
    NUM_CPU = count_num_cpus() + 1;

    while (1)
    {
    double usage[NUM_CPU];
    calculate_cpu_usage(usage);
    for (int i = 0; i < NUM_CPU; ++i) {
        if (i == 0)
            printf("CPU Total Usage: %.2f%%\n", usage[i]);
        else
            printf("CPU%d Usage: %.2f%%\n", i - 1, usage[i]);
        }
    sleep(2);        
    }

    return 0;
}