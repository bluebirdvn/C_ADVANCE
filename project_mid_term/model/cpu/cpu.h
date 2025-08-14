#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct CpuManager CpuManager;

void cpu_manager_destroy(void);

CpuManager *cpu_manager_create(void);


void get_usage(CpuManager *cpu_manager);
void get_temperature(CpuManager *cpu_manager);
void get_frequency(CpuManager *cpu_manager);
void get_top_process(CpuManager *cpu_manager);


struct CpuManager {
    double *usage_percent;
    int core_count;
    double frequency_mhz;
    double temperature_celsius;
    char top_process[256];
    
    unsigned long long **cpu_times_prev;
    unsigned long long **cpu_times_cur;
    unsigned long long *idle;          
    unsigned long long *total;

    void (*update_cpu_times)(void);
    void (*update_usage)(void);
    void (*update_temperature)(void);
    void (*update_frequency)(void);
    void (*update_core_count)(void);
    void (*update_top_process)(void);
    void (*calculate_idle_total)(void);
};

#ifdef __cplusplus
}
#endif

#endif // CPU_H_