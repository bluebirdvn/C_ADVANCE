#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <ctype.h>

static SystemManager* g_system_manager_instance = NULL;

// --- Internal Function Declarations ---
static void update_uptime(void);
static void update_time(void);
static void update_kernel(void);
static void update_load_avg(void);
static void update_proc_count(void);
static int is_numeric_name(const char *s);

void system_manager_destroy(void) 
{
    if (g_system_manager_instance) {
        free(g_system_manager_instance);
        g_system_manager_instance = NULL;
    }
}


SystemManager *system_manager_create(void) 
{
    if (g_system_manager_instance == NULL) {
        g_system_manager_instance = (SystemManager*)calloc(1, sizeof(SystemManager));
        if (!g_system_manager_instance) {
            perror("Failed to allocate memory for SystemManager");
            return NULL;
        }
        
        g_system_manager_instance->update_uptime = update_uptime;
        g_system_manager_instance->update_time = update_time;
        g_system_manager_instance->update_kernel = update_kernel;
        g_system_manager_instance->update_load_avg = update_load_avg;
        g_system_manager_instance->update_proc_count = update_proc_count;
    }
    return g_system_manager_instance;
}

static void update_uptime(void) 
{
    if (!g_system_manager_instance) return;
    FILE *fp = fopen("/proc/uptime", "r");
    if (!fp) return;
    
    double up_seconds = 0.0;
    if (fscanf(fp, "%lf", &up_seconds) == 1) {
        int secs = (int)up_seconds;
        g_system_manager_instance->uptime.days = secs / 86400;
        secs %= 86400;
        g_system_manager_instance->uptime.hours = secs / 3600;
        secs %= 3600;
        g_system_manager_instance->uptime.minutes = secs / 60;
    }
    fclose(fp);
}

static void update_time(void) 
{
    if (!g_system_manager_instance) return;
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    strftime(g_system_manager_instance->clock.iso_datetime, 
             sizeof(g_system_manager_instance->clock.iso_datetime), 
             "%Y-%m-%d %H:%M:%S", &tm_info);
}

static void update_kernel(void) 
{
    if (!g_system_manager_instance) return;
    struct utsname u;
    if (uname(&u) == 0) {
        strncpy(g_system_manager_instance->kernel.kernel_release, u.release, sizeof(g_system_manager_instance->kernel.kernel_release) - 1);
        strncpy(g_system_manager_instance->kernel.machine, u.machine, sizeof(g_system_manager_instance->kernel.machine) - 1);
    }
}

static int is_numeric_name(const char *s) 
{
    if (!s || !*s) return 0;
    for (const char *p = s; *p; ++p) if (!isdigit((unsigned char)*p)) return 0;
    return 1;
}

static void update_proc_count(void) 
{
    if (system_manager_instance == NULL) return;

    DIR *d = opendir("/proc");
    if (!d) { perror("opendir /proc"); return; }
    struct dirent *de;
    unsigned int cnt = 0;
    while ((de = readdir(d)) != NULL) {
        if (de->d_type == DT_DIR && is_numeric_name(de->d_name)) cnt++;
    }
    closedir(d);
    g_system_manager_instance.proc.process_count = cnt; 
}

static void update_load_avg(void) 
{
    if (system_manager_instance == NULL) return;

    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) { perror("/proc/loadavg"); return; }
    double l1=0,l5=0,l15=0;
    if (fscanf(fp, "%lf %lf %lf", &l1, &l5, &l15) == 3) {
        g_system_manager_instance.load.load1 = l1;
        g_system_manager_instance.load.load5 = l5;
        g_system_manager_instance.load.load15 = l15;
    }
    fclose(fp);
}

// --- Public API ---
void system_update_uptime(SystemManager *manager) 
{
    if(manager) manager->update_uptime();
}

void system_update_time(SystemManager *manager) 
{
    if(manager) manager->update_time();
}

void system_update_kernel_info(SystemManager *manager) 
{
    if(manager) manager->update_kernel();
}

void system_update_load_average(SystemManager *manager) 
{
    if(manager) manager->update_load_avg();
}

void system_update_process_count(SystemManager *manager) 
{
    if(manager) manager->update_proc_count();
}