
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <ctype.h>
// #include <time.h>

// #include "cpu.h"

// struct CPU_times{
//     uint64_t cpu_times[NUM_FIELDS];
// };

// struct CPU_manager{
//     CPU_times start;
//     CPU_times end;
//     uint64_t idle;
//     uint64_t total;
//     double usage;
//     uint8_t num_cpu;

//     double temperature;
//     double frequency;

//     void (*count_num_cpus)(CPU_manager *cpu_manager);
//     void (*cpu_get_times)(CPU_manager *cpu_manager);
//     void (*calculate_idle_and_total)(CPU_manager *cpu_manager);
//     void (*calculate_cpu_usage)(CPU_manager *cpu_manager);
//     void (*simulation_temperature)(void);
//     void (*get_cpu_temperature)(CPU_manager *cpu_manager);
//     void (*get_cpu_frequency)(CPU_manager *cpu_manager);
//     void (*get_top_processes_by_cpu)(void);
// };


// void count_num_cpus(CPU_manager *cpu_manager)
// {
//     if (cpu_manager == NULL) {
//         perror("cpu_manager is NULL ptr");
//         EXIT_FAILURE;
//     }
//     FILE *fp = fopen("/proc/stat", "r");
//     if (!fp) {
//         perror("Failed to open /proc/stat");
//         return -1;
//     }

//     int num_cpus = 0;
//     char line[256];
//     while (fgets(line, sizeof(line), fp)) {
//         if (strncmp(line, "cpu", 3) == 0 && isdigit(line[3])) {
//             num_cpus++;
//         } 
//     }
//     fclose(fp);
//     cpu_manager->num_cpu = num_cpus + 1;
// }

// void cpu_get_times(CPU_manager *cpu_manager)
// {
//     if (cpu_manager == NULL) {
//         perror("cpu_manager is NULL ptr");
//         EXIT_FAILURE;
//     }

//     FILE *fp = fopen("/proc/stat", "r");
//     if (!fp) {
//         perror("Failed to open /proc/stat");
//         return;
//     }
    
//     char buffer[256];
    
//     while (fgets(buffer, sizeof(buffer), fp)) {

//         if (strncmp(buffer, "cpu ", 4) == 0) {
//             sscanf(buffer, "cpu  %lu %lu %lu %lu %lu %lu %lu %lu",
//                    &cpu_manager->end[0].end.cpu_times[0], &cpu_usage[0].end.cpu_times[1],
//                    &cpu_usage[0].end.cpu_times[2], &cpu_usage[0].end.cpu_times[3],
//                    &cpu_usage[0].end.cpu_times[4], &cpu_usage[0].end.cpu_times[5],
//                    &cpu_usage[0].end.cpu_times[6], &cpu_usage[0].end.cpu_times[7]);
//         } else if (strncmp(buffer, "cpu", 3) == 0 && isdigit(buffer[3])) {

//             int core_id = -1; 
//             uint64_t times[NUM_FIELDS];
//             int ret = sscanf(buffer, "cpu%d %lu %lu %lu %lu %lu %lu %lu %lu",
//                              &core_id,
//                              &times[0], &times[1], &times[2], &times[3],
//                              &times[4], &times[5], &times[6], &times[7]);
                             

//             if (ret == (NUM_FIELDS + 1) && core_id >= 0 && (core_id + 1) < num_cpus) {
//                 memcpy(cpu_usage[core_id + 1].end.cpu_times, times, sizeof(times));
//             }
//         } 
//         else {
//             break;
//         }
//     } 
    
//     fclose(fp);
// }

// void calculate_idle_and_total(CPU_usage *usage) 
// {
//     uint64_t *s = usage->start.cpu_times;
//     uint64_t *e = usage->end.cpu_times;

//     uint64_t idle_start = s[3] + s[4];
//     uint64_t idle_end   = e[3] + e[4];
//     uint64_t total_start = 0, total_end = 0;

//     for (int i = 0; i < NUM_FIELDS; ++i) {
//         total_start += s[i];
//         total_end   += e[i];
//     }

//     usage->idle = idle_end - idle_start;
//     usage->total = total_end - total_start;
// }

// void calculate_cpu_usage(CPU_usage *usage, int num_cpus)
// {
//     for (int i = 0; i < num_cpus; ++i) {
//         calculate_idle_and_total(&usage[i]);
//         if (usage[i].total == 0)
//             usage[i].usage = 0.0;
//         else
//             usage[i].usage = 100.0 * (1.0 - ((double)usage[i].idle / usage[i].total));
//     }
// }


// void simulation_temperature(void) 
// {
//     FILE *fp = fopen("/home/shunkun/fake_temp.txt", "w");
//     if (!fp) {
//         perror("Failed to create simulation temperature file");
//         return;
//     }

//     srand(time(NULL));

//     double simulated_temp = 30 + rand()%70;
//     fprintf(fp, "%.2f\n", simulated_temp);
//     fclose(fp);
// }


// void get_cpu_temperature(double *temperature)
// {
//     FILE *fp = fopen("/home/shunkun/fake_temp.txt", "r");
//     if (!fp) {
//         perror("Failed to open simulation temperature file");
//         return;
//     }
//     if (fscanf(fp, "%lf", temperature) != 1) {
//         perror("Failed to read CPU temperature");
//         fclose(fp);
//         return;
//     }
//     fclose(fp);
// }

// void get_cpu_frequency(double *frequency)
// {

//     FILE *fp = fopen("/proc/cpuinfo", "r");
//     if (!fp) {
//         perror("Failed to open /proc/cpuinfo");
//         return;

//     }
//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), fp)) {
//         if (strncmp(buffer, "cpu MHz", 7) == 0) {
//             sscanf(buffer, "cpu MHz : %lf", frequency);
//             fclose(fp);
//             return;
//         }
//     }
//     fclose(fp);

// }

// void get_top_processes_by_cpu() {
//     printf("Top 5 processes by CPU usage:\n");
//     system("ps -eo pid,comm,%cpu --sort=-%cpu | head -n 6");
// }


// // int main(void) {
// //     int num_cpus = count_num_cpus();
// //     if (num_cpus < 0) {
// //         fprintf(stderr, "Error counting CPUs\n");
// //         return EXIT_FAILURE;
// //     }

// //     CPU_usage *usage = malloc(sizeof(CPU_usage) * num_cpus);
// //     if (!usage) {
// //         perror("Failed to allocate memory for CPU usage");
// //         return EXIT_FAILURE;
// //     }

// //     cpu_get_times(usage, num_cpus);
    
// //     while (1) {
// //         for (int i = 0; i < num_cpus; ++i) {
// //             memcpy(usage[i].start.cpu_times, usage[i].end.cpu_times, sizeof(usage[i].end.cpu_times));
// //         }

// //         sleep(2);
// //         cpu_get_times(usage, num_cpus);
// //         calculate_cpu_usage(usage, num_cpus);
// //         printf("\n=====================CPU ===================\n");
// //         for (int i = 0; i < num_cpus; ++i) {
// //             if (i ==0) {
// //                 printf("Total CPU Usage: %.2f%%\n", usage[i].usage);
// //             } else {
// //                 printf("CPU %d Usage: %.2f%%\n", i - 1, usage[i].usage);
// //             }

// //         }

// //         double temperature;
// //         simulation_temperature();
// //         get_cpu_temperature(&temperature);
// //         printf("CPU Temperature: %.2f°C\n", temperature);

// //         double frequency;
// //         get_cpu_frequency(&frequency);
// //         if (frequency < 0) {
// //             fprintf(stderr, "Error getting CPU frequency\n");
// //             free(usage);
// //             return EXIT_FAILURE;
// //         }
// //         printf("CPU Frequency: %.2f MHz\n", frequency);
// //         get_top_processes_by_cpu();
// //         sleep(2);
// //     }
// //     free(usage);
// //     return EXIT_SUCCESS;
// // }

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include "cpu.h"

#ifndef NUM_FIELDS
#define NUM_FIELDS 8  // user, nice, system, idle, iowait, irq, softirq, steal
#endif

typedef struct CPU_times {
    uint64_t cpu_times[NUM_FIELDS];
} CPU_times;

typedef struct CPU_manager {
    CPU_times start;
    CPU_times end;
    uint64_t idle;
    uint64_t total;
    double usage;
    uint8_t num_cpu;

    double temperature;
    double frequency;

    void (*count_num_cpus)(struct CPU_manager *cpu_manager);
    void (*cpu_get_times)(struct CPU_manager *cpu_manager);
    void (*calculate_idle_and_total)(struct CPU_manager *cpu_manager);
    void (*calculate_cpu_usage)(struct CPU_manager *cpu_manager);
    void (*simulation_temperature)(void);
    void (*get_cpu_temperature)(struct CPU_manager *cpu_manager);
    void (*get_cpu_frequency)(struct CPU_manager *cpu_manager);
    void (*get_top_processes_by_cpu)(void);
} CPU_manager;

/* ---------- Forward decls (để gán vào function pointers) ---------- */
static void mgr_count_num_cpus(CPU_manager *cpu_manager);
static void mgr_cpu_get_times(CPU_manager *cpu_manager);
static void mgr_calculate_idle_and_total(CPU_manager *cpu_manager);
static void mgr_calculate_cpu_usage(CPU_manager *cpu_manager);
static void mgr_simulation_temperature(void);
static void mgr_get_cpu_temperature(CPU_manager *cpu_manager);
static void mgr_get_cpu_frequency(CPU_manager *cpu_manager);
static void mgr_get_top_processes_by_cpu(void);

/* ---------- Helper: đọc dòng "cpu " vào CPU_times ---------- */
static int read_total_cpu_times(CPU_times *out)
{
    if (!out) return -1;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return -1;
    }

    char buffer[256];
    int ok = -1;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "cpu ", 4) == 0) {
            /* Tổng CPU */
            int ret = sscanf(buffer, "cpu  %lu %lu %lu %lu %lu %lu %lu %lu",
                             &out->cpu_times[0], &out->cpu_times[1],
                             &out->cpu_times[2], &out->cpu_times[3],
                             &out->cpu_times[4], &out->cpu_times[5],
                             &out->cpu_times[6], &out->cpu_times[7]);
            if (ret == NUM_FIELDS) ok = 0;
            break;
        }
    }
    fclose(fp);
    if (ok != 0) {
        fprintf(stderr, "Failed to parse total cpu line\n");
    }
    return ok;
}

/* ---------- Implementations khớp với struct ---------- */
static void mgr_count_num_cpus(CPU_manager *cpu_manager)
{
    if (!cpu_manager) {
        perror("cpu_manager is NULL ptr");
        return;
    }

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Failed to open /proc/stat");
        return;
    }

    int num_cpus = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu", 3) == 0 && isdigit((unsigned char)line[3])) {
            num_cpus++;
        }
    }
    fclose(fp);

    /* +1 để tính cả tổng "cpu " nếu sau này bạn cần mảng gồm tổng + từng core.
       Nhưng với struct hiện tại (không có mảng), ta chỉ lưu số core thực. */
    cpu_manager->num_cpu = (uint8_t)num_cpus;
}

static void mgr_cpu_get_times(CPU_manager *cpu_manager)
{
    if (!cpu_manager) {
        perror("cpu_manager is NULL ptr");
        return;
    }

    /* Dịch end -> start, rồi đọc mẫu mới vào end */
    memcpy(cpu_manager->start.cpu_times,
           cpu_manager->end.cpu_times,
           sizeof(cpu_manager->end.cpu_times));

    if (read_total_cpu_times(&cpu_manager->end) != 0) {
        /* Không cập nhật được thì thôi */
        return;
    }
}

static void mgr_calculate_idle_and_total(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    uint64_t *s = cpu_manager->start.cpu_times;
    uint64_t *e = cpu_manager->end.cpu_times;

    uint64_t idle_start = s[3] + s[4];  // idle + iowait
    uint64_t idle_end   = e[3] + e[4];

    uint64_t total_start = 0, total_end = 0;
    for (int i = 0; i < NUM_FIELDS; ++i) {
        total_start += s[i];
        total_end   += e[i];
    }

    cpu_manager->idle  = (idle_end  - idle_start);
    cpu_manager->total = (total_end - total_start);
}

static void mgr_calculate_cpu_usage(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    cpu_manager->calculate_idle_and_total(cpu_manager);

    if (cpu_manager->total == 0) {
        cpu_manager->usage = 0.0;
    } else {
        cpu_manager->usage = 100.0 * (1.0 - ((double)cpu_manager->idle / (double)cpu_manager->total));
    }
}

static void mgr_simulation_temperature(void)
{
    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }

    FILE *fp = fopen("/home/shunkun/fake_temp.txt", "w");
    if (!fp) {
        perror("Failed to create simulation temperature file");
        return;
    }

    double simulated_temp = 30.0 + (rand() % 701) / 10.0; // 30.0 .. 100.0
    fprintf(fp, "%.2f\n", simulated_temp);
    fclose(fp);
}

static void mgr_get_cpu_temperature(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    FILE *fp = fopen("/home/shunkun/fake_temp.txt", "r");
    if (!fp) {
        perror("Failed to open simulation temperature file");
        return;
    }
    double t = 0.0;
    if (fscanf(fp, "%lf", &t) != 1) {
        perror("Failed to read CPU temperature");
        fclose(fp);
        return;
    }
    fclose(fp);
    cpu_manager->temperature = t;
}

static void mgr_get_cpu_frequency(CPU_manager *cpu_manager)
{
    if (!cpu_manager) return;

    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }

    char buffer[256];
    double mhz = 0.0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "cpu MHz", 7) == 0) {
            /* Dạng "cpu MHz\t\t: 2593.906" */
            if (sscanf(buffer, "cpu MHz%*[^0-9]%lf", &mhz) == 1) {
                break;
            }
        }
    }
    fclose(fp);

    cpu_manager->frequency = mhz; // MHz
}

static void mgr_get_top_processes_by_cpu(void)
{
    printf("Top 5 processes by CPU usage:\n");
    /* Lưu ý: trên GNU ps dùng 'comm' và '%cpu' (hoặc 'pcpu'). */
    system("ps -eo pid,comm,%cpu --sort=-%cpu | head -n 6");
}

/* ---------- Public: init để gán function pointers ---------- */
void cpu_manager_init(CPU_manager *mgr)
{
    if (!mgr) return;
    memset(mgr, 0, sizeof(*mgr));

    mgr->count_num_cpus           = mgr_count_num_cpus;
    mgr->cpu_get_times            = mgr_cpu_get_times;
    mgr->calculate_idle_and_total = mgr_calculate_idle_and_total;
    mgr->calculate_cpu_usage      = mgr_calculate_cpu_usage;
    mgr->simulation_temperature   = mgr_simulation_temperature;
    mgr->get_cpu_temperature      = mgr_get_cpu_temperature;
    mgr->get_cpu_frequency        = mgr_get_cpu_frequency;
    mgr->get_top_processes_by_cpu = mgr_get_top_processes_by_cpu;

    /* Lấy snapshot đầu tiên để start = end = hiện tại */
    if (read_total_cpu_times(&mgr->end) == 0) {
        memcpy(mgr->start.cpu_times, mgr->end.cpu_times, sizeof(mgr->end.cpu_times));
    }
    mgr->count_num_cpus(mgr);
}

/* ---------- Ví dụ loop sử dụng Manager ----------
int main(void)
{
    CPU_manager mgr;
    cpu_manager_init(&mgr);

    while (1) {
        mgr.cpu_get_times(&mgr);
        mgr.calculate_cpu_usage(&mgr);

        printf("\n===== CPU =====\n");
        printf("Total CPU Usage: %.2f%%\n", mgr.usage);

        mgr.simulation_temperature();
        mgr.get_cpu_temperature(&mgr);
        printf("CPU Temperature: %.2f°C\n", mgr.temperature);

        mgr.get_cpu_frequency(&mgr);
        printf("CPU Frequency: %.2f MHz\n", mgr.frequency);

        mgr.get_top_processes_by_cpu();

        sleep(2);
    }
    return 0;
}
-------------------------------------------------- */
