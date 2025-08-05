#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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
void get_cpu_times(uint64_t *user, uint64_t *nice, uint64_t *system, uint64_t *idle, uint64_t *iowait, uint64_t *irq, uint64_t *softirq, uint64_t *steal);

/*
* @function: calculate_cpu_usage: Calculates the CPU usage percentage based on the CPU times.
* @parameters: start, end: Pointers to variables where the start and end CPU times will be stored.
* idle: Pointer to the variable where the idle time will be stored.
* total: Pointer to the variable where the total CPU time will be stored.
* @return: Returns the CPU idle time and total CPU time.
*/
int calculate_cpu_usage();

/*
* @function: calculate_cpu_uage: Calculates the CPU usage percentage.
* @parameters: None.
* @return: Returns the CPU usage percentage.
*/
int calculate_cpu_usage();

#endif // _CPU_H
