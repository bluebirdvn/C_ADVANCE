
#include "cpu.h"

int count_num_cpus(void)
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return -1;
    }

    int num_cpus = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu", 3) == 0 && isdigit(line[3])) {
            num_cpus++;
        } 
    }
    fclose(fp);

    return num_cpus + 1;
}

void cpu_get_times(CPU_usage *cpu_usage, int num_cpus)
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return;
    }
    
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), fp)) {

        if (strncmp(buffer, "cpu ", 4) == 0) {
            sscanf(buffer, "cpu  %lu %lu %lu %lu %lu %lu %lu %lu",
                   &cpu_usage[0].end.cpu_times[0], &cpu_usage[0].end.cpu_times[1],
                   &cpu_usage[0].end.cpu_times[2], &cpu_usage[0].end.cpu_times[3],
                   &cpu_usage[0].end.cpu_times[4], &cpu_usage[0].end.cpu_times[5],
                   &cpu_usage[0].end.cpu_times[6], &cpu_usage[0].end.cpu_times[7]);
        } else if (strncmp(buffer, "cpu", 3) == 0 && isdigit(buffer[3])) {

            int core_id = -1; 
            uint64_t times[NUM_FIELDS];
            int ret = sscanf(buffer, "cpu%d %lu %lu %lu %lu %lu %lu %lu %lu",
                             &core_id,
                             &times[0], &times[1], &times[2], &times[3],
                             &times[4], &times[5], &times[6], &times[7]);
                             

            if (ret == (NUM_FIELDS + 1) && core_id >= 0 && (core_id + 1) < num_cpus) {
                memcpy(cpu_usage[core_id + 1].end.cpu_times, times, sizeof(times));
            }
        } 
        else {
            break;
        }
    } 
    
    fclose(fp);
}

void calculate_idle_and_total(CPU_usage *usage) 
{
    uint64_t *s = usage->start.cpu_times;
    uint64_t *e = usage->end.cpu_times;

    uint64_t idle_start = s[3] + s[4];
    uint64_t idle_end   = e[3] + e[4];
    uint64_t total_start = 0, total_end = 0;

    for (int i = 0; i < NUM_FIELDS; ++i) {
        total_start += s[i];
        total_end   += e[i];
    }

    usage->idle = idle_end - idle_start;
    usage->total = total_end - total_start;
}

void calculate_cpu_usage(CPU_usage *usage, int num_cpus)
{
    for (int i = 0; i < num_cpus; ++i) {
        calculate_idle_and_total(&usage[i]);
        if (usage[i].total == 0)
            usage[i].usage = 0.0;
        else
            usage[i].usage = 100.0 * (1.0 - ((double)usage[i].idle / usage[i].total));
    }
}


int main(void) {
    int num_cpus = count_num_cpus();
    if (num_cpus < 0) {
        fprintf(stderr, "Error counting CPUs\n");
        return EXIT_FAILURE;
    }

    CPU_usage *usage = malloc(sizeof(CPU_usage) * num_cpus);
    if (!usage) {
        perror("Failed to allocate memory for CPU usage");
        return EXIT_FAILURE;
    }

    cpu_get_times(usage, num_cpus);
    
    while (1) {
        for (int i = 0; i < num_cpus; ++i) {
            memcpy(usage[i].start.cpu_times, usage[i].end.cpu_times, sizeof(usage[i].end.cpu_times));
        }

        sleep(2);
        cpu_get_times(usage, num_cpus);
        calculate_cpu_usage(usage, num_cpus);

        for (int i = 0; i < num_cpus; ++i) {
            if (i ==0) {
                printf("Total CPU Usage: %.2f%%\n", usage[i].usage);
            } else {
                printf("CPU %d Usage: %.2f%%\n", i - 1, usage[i].usage);
            }

        }

    }
    free(usage);
    return EXIT_SUCCESS;
}