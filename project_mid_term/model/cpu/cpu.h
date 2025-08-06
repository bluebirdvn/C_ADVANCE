#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#define NUM_FIELDS 8

typedef struct {
    uint64_t cpu_times[NUM_FIELDS];
} CPU_times;

typedef struct {
    CPU_times start;
    CPU_times end;
    uint64_t idle;
    uint64_t total;
    double usage;
} CPU_usage;


/*
*@function: count_num_cpus
*@description: count the number of CPUs in the system
*@return: the number of CPUs
* this function reads the /proc/stat file to determin the number of CPUs
*/
int count_num_cpus(void);

/*
*@function: get_cpu_times
*@description: get the CPU times from /proc/stat
*@param: cpu_times - an array to store the CPU times 
*@param: num_cpus - the number of CPUs
*/
void cpu_get_times(CPU_usage *cpu_usage, int num_cpus);
/*
*@function: calculate_idle_and_total
*@description: calculate the idle and total CPU times
*@param: CPU_usage - a struct storing the CPU times, idle time (calculated) and total time (calculated), and usage (calculated)
*/
void calculate_idle_and_total(CPU_usage *usage);

/*
*@function: calculate_cpu_usage
*@description: calculate the CPU usage for total and each CPU
*@param: CPU_usage - a struct storing the CPU times, idle time (calculated) and total time (calculated), and usage (calculated)
*@param: num_cpus - the number of CPUs
*/
void calculate_cpu_usage(CPU_usage *usage, int num_cpus);

/*
*@function: simulation_temperature
*@description: simulate the temperature of the CPU
* this function create a file named "temperature.txt" is put in the /home/shunkun/fake_temp.txt file.
*/
void simulation_temperature(void);
/*
*@function: get_cpu_temperature
*@description: get the CPU temperature from the file /home/shunkun/fake_temp.txt
*@param: temperature - a pointer to store the CPU temperature.
*/
void get_cpu_temperature(double *temperature);


/*
* @function: get_cpu_frequency
* @descripton: get the CPU frequency from the file /proc/cpuinfo
* @param: frequency - a pointer to store the CPU frequency.
*/
void get_cpu_frequency(double *frequency);

/*
*@function:get_top_processes_by_cpu
*@description: get the top processes by CPU usage
* this function use system command to get the top processes by CPU usage
* and print the result to the console.
*/
void get_top_processes_by_cpu(void);
#endif // _CPU_H
