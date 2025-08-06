#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>


int NUM_CPU;
/*
* @function: count_num_cpus: counts the number of CPU cores available on the system
* @parameters: None.
* @return: Returns the number of CPU cores.
*/
int count_num_cpus(void);
/*
* @function: get_cpu_times: Reads the CPU times from the /proc/stat file.
* @parameters: user, nice, system, idle, iowait, irq, softirq: Pointers to variables where the CPU times will be stored.
* user: Time spent in user mode.
* nice: Time spent in user mode with low priority (nice).
* system: Time spent in system mode.
* idle: Time spent in idle mode.
* iowait: Time spent waiting for I/O operations.
* irq: Time spent servicing hardware interrupts.
* softirq: Time spent servicing software interrupts.
* @return: None.
*/
// void get_cpu_times(uint64_t *user, uint64_t *nice, uint64_t *system, uint64_t *idle, uint64_t *iowait, uint64_t *irq, uint64_t *softirq, uint64_t *steal);
void get_cpu_times(uint64_t cpu_times[NUM_CPU][8]);
/*
* @function: calculate_cpu_usage: Calculates the CPU usage percentage based on the CPU times.
* @parameters: start, end: Pointers to variables where the start and end CPU times will be stored.
* idle: Pointer to the variable where the idle time will be stored.
* total: Pointer to the variable where the total CPU time will be stored.
* @return: Returns the CPU idle time and total CPU time.
*/



/*
* @function: calculate_cpu_uage: Calculates the CPU usage percentage.
* @parameters: None.
* @return: Returns the CPU usage percentage.
*/
void calculate_cpu_usage(double *usage);


/*
* @function: calculate_each_cpu_uasge: Calculates the CPU usage for each CPU core.
* @parameters: idle: Pointer to the variable where the idle time for each CPU core will be stored.
* total: Pointer to the variable where the total CPU time for each CPU core will be stored.
* usage: Pointer to the variable where the CPU usage percentage for each CPU core will be stored.
* @return: None.
*/
void calculate_each_cpu_uage(uint64_t *idle, uint64_t *total, double *usage);



#endif // _CPU_H
