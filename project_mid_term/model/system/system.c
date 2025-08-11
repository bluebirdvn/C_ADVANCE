#include "system.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <ctype.h>

struct System_info {
    // Snapshots
    System_uptime uptime;
    System_clock clock;
    System_kernel kernel;
    System_load load;
    System_proc proc;

    // Methods
    void (*get_up_time)(System_info *);
    void (*get_system_time)(System_info *);
    void (*get_kernel_version)(System_info *);
    void (*service_info)(System_info *);
    void (*load_average)(System_info *);
};

static struct System_info g_si;
static int g_inited = 0;

static void ensure_init(void) {
    if (g_inited) return;
    memset(&g_si, 0, sizeof(g_si));
    g_si.get_up_time = get_up_time;
    g_si.get_system_time = get_system_time;
    g_si.get_kernel_version = get_kernel_version;
    g_si.service_info = service_info;
    g_si.load_average = load_average;
    g_inited = 1;
}

System_info *system_info_instance(void) {
    ensure_init();
    return &g_si;
}

void get_up_time(System_info *si) {
    (void)si; // singleton
    ensure_init();
    FILE *fp = fopen("/proc/uptime", "r");
    if (!fp) { perror("/proc/uptime"); return; }
    double up = 0.0;
    if (fscanf(fp, "%lf", &up) == 1) {
        g_si.uptime.uptime_seconds = (uint64_t)up;
        int secs = (int)up;
        g_si.uptime.days = secs / 86400;
        g_si.uptime.hours = (secs % 86400) / 3600;
        g_si.uptime.minutes = ((secs % 86400) % 3600) / 60;
    }
    fclose(fp);
}

void get_system_time(System_info *si) {
    (void)si;
    ensure_init();
    time_t now = time(NULL);
    struct tm tmv;
    if (localtime_r(&now, &tmv)) {
        strftime(g_si.clock.iso_datetime, sizeof(g_si.clock.iso_datetime), "%Y-%m-%d %H:%M:%S", &tmv);
    }
}

void get_kernel_version(System_info *si) {
    (void)si;
    ensure_init();
    struct utsname u;
    if (uname(&u) == 0) {
        strncpy(g_si.kernel.kernel_release, u.release, sizeof(g_si.kernel.kernel_release) - 1);
        strncpy(g_si.kernel.sysname, u.sysname, sizeof(g_si.kernel.sysname) - 1);
        strncpy(g_si.kernel.machine, u.machine, sizeof(g_si.kernel.machine) - 1);
    } else {
        perror("uname");
    }
}

static int is_numeric_name(const char *s) {
    if (!s || !*s) return 0;
    for (const char *p = s; *p; ++p) if (!isdigit((unsigned char)*p)) return 0;
    return 1;
}

void service_info(System_info *si) {
    (void)si;
    ensure_init();
    DIR *d = opendir("/proc");
    if (!d) { perror("opendir /proc"); return; }
    struct dirent *de;
    unsigned int cnt = 0;
    while ((de = readdir(d)) != NULL) {
        if (de->d_type == DT_DIR && is_numeric_name(de->d_name)) cnt++;
    }
    closedir(d);
    g_si.proc.process_count = cnt; // why: avoids invoking external `ps`
}

void load_average(System_info *si) {
    (void)si;
    ensure_init();
    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) { perror("/proc/loadavg"); return; }
    double l1=0,l5=0,l15=0;
    if (fscanf(fp, "%lf %lf %lf", &l1, &l5, &l15) == 3) {
        g_si.load.load1 = l1;
        g_si.load.load5 = l5;
        g_si.load.load15 = l15;
    }
    fclose(fp);
}