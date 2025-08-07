#ifndef _SYSTEM_H
#define _SYSTEM_H

/*
 * @function: get_up_time
 * @description: Retrieve the system uptime by reading from /proc/uptime
 */
void get_up_time(void);

/*
 * @function: get_system_time
 * @description: Retrieve the current system time using time() function
 */
void get_system_time(void);

/*
 * @function: get_kernel_version
 * @description: Retrieve the kernel version using uname()
 */
void get_kernel_version(void);

/*
 * @function: service_info
 * @description: Retrieve a list or count of running services/processes using ps
 */
void service_info(void);

/*
 * @function: load_average
 * @description: Retrieve load average values (1, 5, 15 minutes) from /proc/loadavg
 */
void load_average(void);

#endif //_SYSTEM_H
