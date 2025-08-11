#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Opaque singleton type
typedef struct System_info System_info;

// Data snapshots
typedef struct {
    uint64_t uptime_seconds;
    int days;
    int hours;
    int minutes;
} System_uptime;

typedef struct {
    char iso_datetime[32]; // YYYY-MM-DD HH:MM:SS
} System_clock;

typedef struct {
    char kernel_release[128];
    char sysname[64];
    char machine[64];
} System_kernel;

typedef struct {
    double load1;
    double load5;
    double load15;
} System_load;

typedef struct {
    uint32_t process_count; // number of running processes (count /proc numeric entries)
} System_proc;

// Singleton accessor
System_info *system_info_instance(void);

// Public API (also available as function pointers inside the singleton)
void get_up_time(System_info *si);
void get_system_time(System_info *si);
void get_kernel_version(System_info *si);
void service_info(System_info *si);
void load_average(System_info *si);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_H_