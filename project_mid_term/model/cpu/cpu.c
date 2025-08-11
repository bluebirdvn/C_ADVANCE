
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include "cpu.h"

#ifndef NUM_FIELDS
#define NUM_FIELDS 8  
#endif
 
struct CPU_times {
    uint64_t cpu_times[NUM_FIELDS];
};

struct CPU_manager {
    CPU_times start;
    CPU_times end;
    uint64_t idle;
    uint64_t total;
    double usage;
    uint8_t num_cpu;

    double temperature;
    double frequency;

    void (*count_num_cpus)(struct CPU_manager *cpu_manager);
    void (*cpu_get_times)(struct CPU_manager *cpu_manager);
    void (*calculate_idle_and_total)(struct CPU_manager *cpu_manager);
    void (*calculate_cpu_usage)(struct CPU_manager *cpu_manager);
    void (*simulation_temperature)(void);
    void (*get_cpu_temperature)(struct CPU_manager *cpu_manager);
    void (*get_cpu_frequency)(struct CPU_manager *cpu_manager);
    void (*get_top_processes_by_cpu)(void);
};

static CPU_manager cpu_manager_instance;
static int is_initialized = 0;

static void mgr_count_num_cpus(CPU_manager *cpu_manager);
static void mgr_cpu_get_times(CPU_manager *cpu_manager);
static void mgr_calculate_idle_and_total(CPU_manager *cpu_manager);
static void mgr_calculate_cpu_usage(CPU_manager *cpu_manager);
static void mgr_simulation_temperature(void);
static void mgr_get_cpu_temperature(CPU_manager *cpu_manager);
static void mgr_get_cpu_frequency(CPU_manager *cpu_manager);
static void mgr_get_top_processes_by_cpu(void);

static int read_total_cpu_times(CPU_times *out)
{
    if (!out) return -1;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return -1;
    }

    char buffer[256];
    int ok = -1;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "cpu ", 4) == 0) {

            int ret = sscanf(buffer, "cpu  %lu %lu %lu %lu %lu %lu %lu %lu",
                             &out->cpu_times[0], &out->cpu_times[1],
                             &out->cpu_times[2], &out->cpu_times[3],
                             &out->cpu_times[4], &out->cpu_times[5],
                             &out->cpu_times[6], &out->cpu_times[7]);
            if (ret == NUM_FIELDS) ok = 0;
            break;
        }
    }
    fclose(fp);
    if (ok != 0) {
        fprintf(stderr, "Failed to parse total cpu line\n");
    }
    return ok;
}


static void mgr_count_num_cpus(CPU_manager *cpu_manager)
{
    if (!cpu_manager) {
        perror("cpu_manager is NULL ptr");
        return;
    }

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return;
    }

    int num_cpus = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu", 3) == 0 && isdigit((unsigned char)line[3])) {
            num_cpus++;
        }
    }
    fclose(fp);


    cpu_manager->num_cpu = (uint8_t)num_cpus;
}

static void mgr_cpu_get_times(CPU_manager *cpu_manager)
{
    if (!cpu_manager) {
        perror("cpu_manager is NULL ptr");
        return;
    }

    memcpy(cpu_manager->start.cpu_times,
           cpu_manager->end.cpu_times,
           sizeof(cpu_manager->end.cpu_times));

    if (read_total_cpu_times(&cpu_manager->end) != 0) {/
        return;
    }
}

static void mgr_calculate_idle_and_total(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    uint64_t *s = cpu_manager->start.cpu_times;
    uint64_t *e = cpu_manager->end.cpu_times;

    uint64_t idle_start = s[3] + s[4];  
    uint64_t idle_end   = e[3] + e[4];

    uint64_t total_start = 0, total_end = 0;
    for (int i = 0; i < NUM_FIELDS; ++i) {
        total_start += s[i];
        total_end   += e[i];
    }

    cpu_manager->idle  = (idle_end  - idle_start);
    cpu_manager->total = (total_end - total_start);
}

static void mgr_calculate_cpu_usage(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    cpu_manager->calculate_idle_and_total(cpu_manager);

    if (cpu_manager->total == 0) {
        cpu_manager->usage = 0.0;
    } else {
        cpu_manager->usage = 100.0 * (1.0 - ((double)cpu_manager->idle / (double)cpu_manager->total));
    }
}

static void mgr_simulation_temperature(void)
{
    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }

    FILE *fp = fopen("/home/shunkun/fake_temp.txt", "w");
    if (!fp) {
        perror("Failed to create simulation temperature file");
        return;
    }

    double simulated_temp = 30.0 + (rand() % 701) / 10.0; 
    fprintf(fp, "%.2f\n", simulated_temp);
    fclose(fp);
}

static void mgr_get_cpu_temperature(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    FILE *fp = fopen("/home/shunkun/fake_temp.txt", "r");
    if (!fp) {
        perror("Failed to open simulation temperature file");
        return;
    }
    double t = 0.0;
    if (fscanf(fp, "%lf", &t) != 1) {
        perror("Failed to read CPU temperature");
        fclose(fp);
        return;
    }
    fclose(fp);
    cpu_manager->temperature = t;
}

static void mgr_get_cpu_frequency(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }

    char buffer[256];
    double mhz = 0.0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "cpu MHz", 7) == 0) {

            if (sscanf(buffer, "cpu MHz%*[^0-9]%lf", &mhz) == 1) {
                break;
            }
        }
    }
    fclose(fp);

    cpu_manager->frequency = mhz;
}

static void mgr_get_top_processes_by_cpu(void)
{
    printf("Top 5 processes by CPU usage:\n");

    system("ps -eo pid,comm,%cpu --sort=-%cpu | head -n 6");
}


CPU_manager *CPU_manager_init(void)
{
    if (!is_initialized) {
        memset(&cpu_manager_instance, 0, sizeof(CPU_manager));
        cpu_manager_instance.count_num_cpus           = mgr_count_num_cpus;
        cpu_manager_instance.cpu_get_times            = mgr_cpu_get_times;
        cpu_manager_instance.calculate_idle_and_total = mgr_calculate_idle_and_total;
        cpu_manager_instance.calculate_cpu_usage      = mgr_calculate_cpu_usage;
        cpu_manager_instance.simulation_temperature   = mgr_simulation_temperature;
        cpu_manager_instance.get_cpu_temperature      = mgr_get_cpu_temperature;
        cpu_manager_instance.get_cpu_frequency        = mgr_get_cpu_frequency;
        cpu_manager_instance.get_top_processes_by_cpu = mgr_get_top_processes_by_cpu;

        is_initialized = 1;
    }

    if (read_total_cpu_times(&cpu_manager_instance->end) == 0) {
        memcpy(cpu_manager_instance->start.cpu_times, cpu_manager_instance->end.cpu_times, sizeof(cpu_manager_instance->end.cpu_times));
    }
    cpu_manager_instance->count_num_cpus(mgr);

    return &cpu_manager_instance;
}

/* ---------- Ví dụ loop sử dụng Manager ----------
int main(void)
{
    CPU_manager mgr;
    cpu_manager_init(&mgr);

    while (1) {
        mgr.cpu_get_times(&mgr);
        mgr.calculate_cpu_usage(&mgr);

        printf("\n===== CPU =====\n");
        printf("Total CPU Usage: %.2f%%\n", mgr.usage);

        mgr.simulation_temperature();
        mgr.get_cpu_temperature(&mgr);
        printf("CPU Temperature: %.2f°C\n", mgr.temperature);

        mgr.get_cpu_frequency(&mgr);
        printf("CPU Frequency: %.2f MHz\n", mgr.frequency);

        mgr.get_top_processes_by_cpu();

        sleep(2);
    }
    return 0;
}
-------------------------------------------------- */
