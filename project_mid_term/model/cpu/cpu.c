#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#ifndef NUM_FIELDS
#define NUM_FIELDS 8  
#endif


static CpuManager* g_cpu_manager_instance = NULL;

static void update_core_count(void);
static void update_cpu_times(void);
static void calculate_idle_total(void);
static void update_usage(void);
static void update_temperature(void);
static void update_frequency(void);
static void update_top_process(void);
static void copy_cpu_times_cur_to_prev(void);


void cpu_manager_destroy(void) 
{
    if (g_cpu_manager_instance == NULL) return;
    
    // Free arrays
    free(g_cpu_manager_instance->usage_percent);
    free(g_cpu_manager_instance->idle);
    free(g_cpu_manager_instance->total);
    
    // Free 2D arrays
    if (g_cpu_manager_instance->cpu_times_cur != NULL) {
        for (int i = 0; i < g_cpu_manager_instance->core_count; ++i) {
            free(g_cpu_manager_instance->cpu_times_cur[i]);
        }
        free(g_cpu_manager_instance->cpu_times_cur);
    }
    
    if (g_cpu_manager_instance->cpu_times_prev != NULL) {
        for (int i = 0; i < g_cpu_manager_instance->core_count; ++i) {
            if (g_cpu_manager_instance->cpu_times_prev[i] != NULL) {
                free(g_cpu_manager_instance->cpu_times_prev[i]);
            }
        }
        free(g_cpu_manager_instance->cpu_times_prev);
    }
    
    // Finally free the main structure
    free(g_cpu_manager_instance);
    g_cpu_manager_instance = NULL;
}


static void update_core_count(void)
{
    if (g_cpu_manager_instance == NULL) return;
    // The +1 is for the aggregate "cpu" line in /proc/stat
    g_cpu_manager_instance->core_count = sysconf(_SC_NPROCESSORS_ONLN) + 1;
}

static void update_cpu_times(void)
{
    if (g_cpu_manager_instance == NULL) return;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("fopen /proc/stat");
        return;
    }

    char line[256];
    int current_core_idx = 0;

    while (fgets(line, sizeof(line), fp) && current_core_idx < g_cpu_manager_instance->core_count) {
        if (strncmp(line, "cpu", 3) == 0) {
            // Check if it's the aggregate "cpu " or a specific core "cpuN "
            if (line[3] == ' ' || isdigit((unsigned char)line[3])) {
                unsigned long long vals[NUM_FIELDS] = {0};
                
                // For "cpu ", we parse directly
                if (line[3] == ' ') {
                     sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
                           &vals[0], &vals[1], &vals[2], &vals[3],
                           &vals[4], &vals[5], &vals[6], &vals[7]);
                } else { // For "cpuN", we parse with the core number
                    // MODIFIED: Correctly ignore the core number in sscanf
                    sscanf(line, "cpu%*u %llu %llu %llu %llu %llu %llu %llu %llu",
                           &vals[0], &vals[1], &vals[2], &vals[3],
                           &vals[4], &vals[5], &vals[6], &vals[7]);
                }

                for (int f = 0; f < NUM_FIELDS; f++) {
                    g_cpu_manager_instance->cpu_times_cur[current_core_idx][f] = vals[f];
                }
                current_core_idx++;
            }
        } else {
            break; // No more cpu lines
        }
    }

    fclose(fp);
}

static void calculate_idle_total(void) 
{
    if (g_cpu_manager_instance == NULL) return;

    for (int i = 0; i < g_cpu_manager_instance->core_count; ++i) {
        unsigned long long sum_prev = 0;
        for (int j = 0; j < NUM_FIELDS; j++) {
            sum_prev += g_cpu_manager_instance->cpu_times_prev[i][j];
        }
        
        unsigned long long sum_cur = 0;
        for (int j = 0; j < NUM_FIELDS; j++) {
            sum_cur += g_cpu_manager_instance->cpu_times_cur[i][j];
        }
        
        g_cpu_manager_instance->total[i] = sum_cur - sum_prev;
        g_cpu_manager_instance->idle[i] = (g_cpu_manager_instance->cpu_times_cur[i][3] + g_cpu_manager_instance->cpu_times_cur[i][4]) -
                                          (g_cpu_manager_instance->cpu_times_prev[i][3] + g_cpu_manager_instance->cpu_times_prev[i][4]);
    }
}


static void update_usage(void) 
{
    if (g_cpu_manager_instance == NULL) return;

    for (int i = 0; i < g_cpu_manager_instance->core_count; ++i) {
        unsigned long long total_diff = g_cpu_manager_instance->total[i];
        unsigned long long idle_diff = g_cpu_manager_instance->idle[i];
        
        if (total_diff == 0) {
            g_cpu_manager_instance->usage_percent[i] = 0.0;
        } else {
            // MODIFIED: Fixed syntax and logic
            double idle_ratio = (double)idle_diff / (double)total_diff;
            g_cpu_manager_instance->usage_percent[i] = 100.0 * (1.0 - idle_ratio);
        }
        // Clamp values between 0 and 100
        if (g_cpu_manager_instance->usage_percent[i] < 0.0) g_cpu_manager_instance->usage_percent[i] = 0.0;
        if (g_cpu_manager_instance->usage_percent[i] > 100.0) g_cpu_manager_instance->usage_percent[i] = 100.0;
    }    
}

static void update_frequency(void)
{
    if (g_cpu_manager_instance == NULL) return;

    FILE* file = fopen("/proc/cpuinfo", "r");
    if (!file) return;
    
    char line[256];
    double total_freq = 0;
    int core_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "cpu MHz")) {
            double freq;
            if (sscanf(line, "cpu MHz\t: %lf", &freq) == 1) {
                total_freq += freq;
                core_count++;
            }
        }
    }
    fclose(file);
    if (core_count > 0) {
        g_cpu_manager_instance->frequency_mhz = total_freq / core_count;
    }
}


static void update_temperature(void)
{
    if (g_cpu_manager_instance == NULL) return;
    g_cpu_manager_instance->temperature_celsius = 40.0 + (rand() % 30);
}

static void update_top_process(void)
{
    if (g_cpu_manager_instance == NULL) return;

    FILE* pipe = popen("ps -eo comm,%cpu --sort=-%cpu --no-headers | head -1", "r");
    if (pipe) {
        if (fgets(g_cpu_manager_instance->top_process, sizeof(g_cpu_manager_instance->top_process), pipe) != NULL) {
            char* newline = strchr(g_cpu_manager_instance->top_process, '\n');
            if (newline) *newline = '\0';
        }
        pclose(pipe);
    }
}

static void copy_cpu_times_cur_to_prev(void) 
{
    if (g_cpu_manager_instance == NULL) return;
    for (int i = 0; i < g_cpu_manager_instance->core_count; ++i) {
        memcpy(g_cpu_manager_instance->cpu_times_prev[i], 
               g_cpu_manager_instance->cpu_times_cur[i], 
               sizeof(unsigned long long) * NUM_FIELDS);
    }
}


CpuManager *cpu_manager_create(void)
{
    if (g_cpu_manager_instance != NULL) {
        return g_cpu_manager_instance;
    }
    
    g_cpu_manager_instance = malloc(sizeof(CpuManager));
    if (g_cpu_manager_instance == NULL) {
        perror("malloc for CpuManager failed");
        return NULL;
    }
    memset(g_cpu_manager_instance, 0, sizeof(CpuManager));

    g_cpu_manager_instance->update_core_count = update_core_count;
    g_cpu_manager_instance->update_core_count(); // Get core count first

    int core_count = g_cpu_manager_instance->core_count;
    
    // Allocate all memory
    g_cpu_manager_instance->usage_percent = calloc(core_count, sizeof(double));
    g_cpu_manager_instance->idle = calloc(core_count, sizeof(unsigned long long));
    g_cpu_manager_instance->total = calloc(core_count, sizeof(unsigned long long));
    g_cpu_manager_instance->cpu_times_cur = malloc(sizeof(unsigned long long*) * core_count);
    g_cpu_manager_instance->cpu_times_prev = malloc(sizeof(unsigned long long*) * core_count);

    // Check for allocation failures
    if (!g_cpu_manager_instance->usage_percent || !g_cpu_manager_instance->idle || !g_cpu_manager_instance->total ||
        !g_cpu_manager_instance->cpu_times_cur || !g_cpu_manager_instance->cpu_times_prev) {
        cpu_manager_destroy();
        return NULL;
    }

    for (int i = 0; i < core_count; ++i) {
        g_cpu_manager_instance->cpu_times_cur[i] = calloc(NUM_FIELDS, sizeof(unsigned long long));
        g_cpu_manager_instance->cpu_times_prev[i] = calloc(NUM_FIELDS, sizeof(unsigned long long));
        if (!g_cpu_manager_instance->cpu_times_cur[i] || !g_cpu_manager_instance->cpu_times_prev[i]) {
            cpu_manager_destroy();
            return NULL;
        }
    }

    g_cpu_manager_instance->update_cpu_times = update_cpu_times;
    g_cpu_manager_instance->update_usage = update_usage;
    g_cpu_manager_instance->update_temperature = update_temperature;
    g_cpu_manager_instance->update_frequency = update_frequency; 
    g_cpu_manager_instance->update_top_process = update_top_process;
    g_cpu_manager_instance->calculate_idle_total = calculate_idle_total;      
    
    // Initial read to populate prev times
    g_cpu_manager_instance->update_cpu_times();
    copy_cpu_times_cur_to_prev();
    
    return g_cpu_manager_instance;
}

// Public API functions
void get_usage(CpuManager *cpu_manager)
{
    if (cpu_manager == NULL) return;

    copy_cpu_times_cur_to_prev();
    cpu_manager->update_cpu_times();
    cpu_manager->calculate_idle_total();
    cpu_manager->update_usage();
}

void get_temperature(CpuManager *cpu_manager)
{
    if (cpu_manager == NULL) return;
    cpu_manager->update_temperature();
}

void get_frequency(CpuManager *cpu_manager)
{
    if (cpu_manager == NULL) return;
    cpu_manager->update_frequency();
}

void get_top_process(CpuManager *cpu_manager)
{
    if (cpu_manager == NULL) return;
    cpu_manager->update_top_process();
}