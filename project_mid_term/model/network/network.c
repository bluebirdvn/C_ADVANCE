#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>

static NetworkManager* g_network_instance = NULL;

// --- Internal Function Declarations ---
static void network_update_bytes(void);
static void network_update_speed(void);
static void network_update_connections(void);
static void network_update_interface_info(void);
static int read_u64_from_sysfs(const char* path, uint64_t* out);
static int read_str_from_sysfs(const char* path, char* buf, size_t buflen);


void network_manager_destroy(void) {
    if (g_network_instance) {
        free(g_network_instance);
        g_network_instance = NULL;
    }
}

NetworkManager* network_manager_create() {
    if (g_network_instance == NULL) {
        g_network_instance = (NetworkManager*)calloc(1, sizeof(NetworkManager));
        if (!g_network_instance) {
            perror("Failed to allocate memory for NetworkManager");
            return NULL;
        }

        g_network_instance->update_bytes = network_update_bytes;
        g_network_instance->update_speed = network_update_speed;
        g_network_instance->update_connections = network_update_connections;
        g_network_instance->update_interface_info = network_update_interface_info;

        // Try to find the default interface from /proc/net/route
        FILE* file = fopen("/proc/net/route", "r");
        if (file) {
            char line[256], iface[32], dest[32];
            fgets(line, sizeof(line), file); // skip header
            while (fgets(line, sizeof(line), file)) {
                if (sscanf(line, "%31s %31s", iface, dest) == 2) {
                    if (strcmp(dest, "00000000") == 0) { // default route
                        strncpy(g_network_instance->interface_name, iface, sizeof(g_network_instance->interface_name)-1);
                        break;
                    }
                }
            }
            fclose(file);
        }
        // Fallback if not found
        if (g_network_instance->interface_name[0] == '\0') {
            strcpy(g_network_instance->interface_name, "eth0");
        }

        // Get baseline counters + timestamp for speed calculation
        g_network_instance->update_bytes();
        clock_gettime(CLOCK_MONOTONIC, &g_network_instance->last_ts);
        g_network_instance->prev_rx_bytes = g_network_instance->rx_bytes;
        g_network_instance->prev_tx_bytes = g_network_instance->tx_bytes;
    }
    return g_g_network_instance;
}

static void network_update_bytes(void) 
{
    if (!g_g_network_instance) return;
    char path[256];
    const char* ifn = g_g_network_instance->interface_name;

    // bytes
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_bytes", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->rx_bytes);
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_bytes", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->tx_bytes);

    // packets
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_packets", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->rx_packets);
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_packets", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->tx_packets);

    // errors & dropped
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_errors", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->rx_errs);
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_errors", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->tx_errs);

    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_dropped", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->rx_dropped);
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_dropped", ifn);
    (void)read_u64_from_sysfs(path, &g_network_instance->tx_dropped);
}

static void network_update_speed(void) 
{
    if (!g_network_instance) return;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    if (g_network_instance->last_ts.tv_sec != 0 || g_network_instance->last_ts.tv_nsec != 0) {
        double dt = (now.tv_sec - g_network_instance->last_ts.tv_sec)
                  + (now.tv_nsec - g_network_instance->last_ts.tv_nsec) / 1e9;
        if (dt > 0) {
            uint64_t drx = (g_network_instance->rx_bytes >= g_network_instance->prev_rx_bytes)
                         ? (g_network_instance->rx_bytes - g_network_instance->prev_rx_bytes) : 0;
            uint64_t dtx = (g_network_instance->tx_bytes >= g_network_instance->prev_tx_bytes)
                         ? (g_network_instance->tx_bytes - g_network_instance->prev_tx_bytes) : 0;

            uint64_t dprx = (g_network_instance->rx_packets >= g_network_instance->prev_rx_packets)
                          ? (g_network_instance->rx_packets - g_network_instance->prev_rx_packets) : 0;
            uint64_t dptx = (g_network_instance->tx_packets >= g_network_instance->prev_tx_packets)
                          ? (g_network_instance->tx_packets - g_network_instance->prev_tx_packets) : 0;

            // kbps = (bytes * 8) / dt / 1000
            g_network_instance->rx_speed_kbps = (double)drx * 8.0 / dt / 1000.0;
            g_network_instance->tx_speed_kbps = (double)dtx * 8.0 / dt / 1000.0;

            // pps
            g_network_instance->rx_pps = (double)dprx / dt;
            g_network_instance->tx_pps = (double)dptx / dt;

            // % sử dụng băng thông nếu biết speed_mbps
            if (g_network_instance->speed_mbps > 0) {
                double max_kbps = (double)g_network_instance->speed_mbps * 1000.0;
                g_network_instance->rx_util_percent = (g_network_instance->rx_speed_kbps / max_kbps) * 100.0;
                g_network_instance->tx_util_percent = (g_network_instance->tx_speed_kbps / max_kbps) * 100.0;
                if (g_network_instance->rx_util_percent < 0) g_network_instance->rx_util_percent = 0;
                if (g_network_instance->tx_util_percent < 0) g_network_instance->tx_util_percent = 0;
            } else {
                g_network_instance->rx_util_percent = 0;
                g_network_instance->tx_util_percent = 0;
            }
        }
    }

    g_network_instance->prev_rx_bytes = g_network_instance->rx_bytes;
    g_network_instance->prev_tx_bytes = g_network_instance->tx_bytes;
    g_network_instance->prev_rx_packets = g_network_instance->rx_packets;
    g_network_instance->prev_tx_packets = g_network_instance->tx_packets;
    g_network_instance->last_ts = now;
}

static void network_update_connections(void) 
{
    if (!g_network_instance) return;
    int total = 0, est = 0;
    count_tcp_from_file("/proc/net/tcp",  &total, &est);
    count_tcp_from_file("/proc/net/tcp6", &total, &est);
    g_network_instance->tcp_connections_total = total;
    g_network_instance->tcp_connections_established = est;
}

static void network_update_interface_info(void) {
    if (!g_network_instance) return;
    char path[256], buf[64];
    const char* ifn = g_network_instance->interface_name;

    // operstate
    snprintf(path, sizeof(path), "/sys/class/net/%s/operstate", ifn);
    if (read_str_from_sysfs(path, buf, sizeof(buf)) == 0) {
        g_network_instance->link_up = (strcmp(buf, "up") == 0) ? 1 : 0;
    } else {
        g_network_instance->link_up = 0;
    }

    // mtu
    snprintf(path, sizeof(path), "/sys/class/net/%s/mtu", ifn);
    (void)read_u32_from_sysfs(path, &g_network_instance->mtu);

    // speed (Mbps), return 0 or -1 (unknow)
    uint32_t sp = 0;
    snprintf(path, sizeof(path), "/sys/class/net/%s/speed", ifn);
    if (read_u32_from_sysfs(path, &sp) == 0 && sp != (uint32_t)-1) {
        g_network_instance->speed_mbps = sp;
    } else {
        g_network_instance->speed_mbps = 0; // unknown
    }
}

static int read_u64_from_sysfs(const char* path, uint64_t* out) 
{
    FILE* f = fopen(path, "r");
    if (!f) return -1;
    if (fscanf(f, "%lu", (unsigned long*)out) != 1) { fclose(f); return -1; }
    fclose(f);
    return 0;
}

static int read_str_from_sysfs(const char* path, char* buf, size_t buflen) {
    FILE* f = fopen(path, "r");
    if (!f) return -1;
    if (!fgets(buf, buflen, f)) { fclose(f); return -1; }
    fclose(f);
    // strip newline
    size_t n = strlen(buf);
    if (n && buf[n-1] == '\n') buf[n-1] = '\0';
    return 0;
}


// --- Public API Implementations ---
void get_bytes(NetworkManager *manager) {
    if (!manager) return;
    manager->update_bytes();
}

void get_speed(NetworkManager *manager) {
    if (!manager) return;
    manager->update_speed();    
}

void get_connections(NetworkManager *manager) {
    if (!manager) return;
    manager->update_connections();     
}

void get_interface_info(NetworkManager *manager) {
    if (!manager) return;
    manager->update_interface_info();
}
