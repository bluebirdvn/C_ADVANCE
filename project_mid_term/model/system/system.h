#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SystemManager SystemManager;

// --- Public Data Structures ---
typedef struct {
    int days;
    int hours;
    int minutes;
} System_uptime;

typedef struct {
    char iso_datetime[32];
} System_clock;

typedef struct {
    char kernel_release[128];
    char machine[64];
} System_kernel;

typedef struct {
    double load1;
    double load5;
    double load15;
} System_load;

typedef struct {
    uint32_t process_count;
} System_proc;


struct SystemManager {
    System_uptime uptime;
    System_clock clock;
    System_kernel kernel;
    System_load load;
    System_proc proc;

    // --- Internal ---
    void (*update_uptime)(void);
    void (*update_time)(void);
    void (*update_kernel)(void);
    void (*update_load_avg)(void);
    void (*update_proc_count)(void);
};

// --- Singleton API ---
void system_manager_destroy(void);
SystemManager *system_manager_create(void);

// --- Public Data Update API ---
void system_update_uptime(SystemManager *manager);
void system_update_time(SystemManager *manager);
void system_update_kernel_info(SystemManager *manager);
void system_update_load_average(SystemManager *manager);
void system_update_process_count(SystemManager *manager);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_H_