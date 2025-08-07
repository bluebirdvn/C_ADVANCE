#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>

void get_up_time(void) {
    FILE *fp = fopen("/proc/uptime", "r");
    if (!fp) {
        perror("Failed to open /proc/uptime");
        return;
    }

    float uptime;
    if (fscanf(fp, "%f", &uptime) == 1) {
        int days = (int)uptime / 86400;
        int hours = ((int)uptime % 86400) / 3600;
        int minutes = (((int)uptime % 86400) % 3600) / 60;
        printf("Uptime: %d days, %d hours, %d minutes\n", days, hours, minutes);
    } else {
        printf("Failed to parse uptime.\n");
    }

    fclose(fp);
}

void get_system_time(void) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (t) {
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
        printf("System time: %s\n", buffer);
    } else {
        printf("Failed to get system time.\n");
    }
}

void get_kernel_version(void) {
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        printf("Kernel Version: %s\n", buffer.release);
    } else {
        perror("uname failed");
    }
}

void service_info(void) {
    FILE *fp = popen("ps -eo comm --no-headers | wc -l", "r");
    if (!fp) {
        perror("Failed to run ps");
        return;
    }

    int count;
    if (fscanf(fp, "%d", &count) == 1) {
        printf("Running processes: %d\n", count);
    } else {
        printf("Failed to count services.\n");
    }

    pclose(fp);
}

void load_average(void) {
    FILE *fp = fopen("/proc/loadavg", "r");
    if (!fp) {
        perror("Failed to open /proc/loadavg");
        return;
    }

    float load1, load5, load15;
    if (fscanf(fp, "%f %f %f", &load1, &load5, &load15) == 3) {
        printf("Load average (1/5/15 min): %.2f / %.2f / %.2f\n", load1, load5, load15);
    } else {
        printf("Failed to read load average.\n");
    }

    fclose(fp);
}

int main() {
    get_up_time();

    get_system_time();

    load_average();

    service_info();

    get_kernel_version();

    return 0;
}