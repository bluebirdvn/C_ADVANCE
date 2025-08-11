#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// --- Internal constants ---
#define NET_MAX_IFS 10
#define DEFAULT_ROUTE_FILE "/proc/net/route"
#define PROC_TCP "/proc/net/tcp"
#define PROC_UDP "/proc/net/udp"

// --- Internal struct definition ---
struct Network_info {
    // State
    char primary_ifname[64];

    // Data
    Network_speed_info speed_info;
    double bandwidth_usage_tx; // 0..1
    double bandwidth_usage_rx; // 0..1
    uint64_t latency_ms;       // not measured in this sample
    Network_packet_info packet_info;
    Network_connection_info connection_info;
    Network_interface_info interface_info[NET_MAX_IFS];

    // Methods (function pointers)
    void (*get_tx_rx_info)(Network_info *);
    void (*get_network_speed)(Network_info *);
    void (*get_bandwidth_usage)(Network_info *);
    void (*get_packet_info)(Network_info *);
    int  (*get_connection_info)(Network_info *);
    int  (*get_network_interface_info)(Network_info *);
};

// --- Singleton instance ---
static Network_info g_inst;
static int g_inited = 0;

// --- Helpers ---
static int read_uint64_from_file(const char *path, uint64_t *out) {
    FILE *f = fopen(path, "r");
    if (!f) return -1; // caller prints errno context
    unsigned long long v = 0ULL; // why: fscanf with %llu is portable for uint64_t
    int n = fscanf(f, "%llu", &v);
    fclose(f);
    if (n != 1) return -2;
    *out = (uint64_t)v;
    return 0;
}

static int read_uint32_from_file(const char *path, uint32_t *out) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    unsigned int v = 0U;
    int n = fscanf(f, "%u", &v);
    fclose(f);
    if (n != 1) return -2;
    *out = (uint32_t)v;
    return 0;
}

static void build_stat_path(char *buf, size_t bufsz, const char *ifname, const char *statfile) {
    // /sys/class/net/<if>/statistics/<statfile>
    snprintf(buf, bufsz, "/sys/class/net/%s/statistics/%s", ifname, statfile);
}

static void build_if_path(char *buf, size_t bufsz, const char *ifname, const char *file) {
    // /sys/class/net/<if>/<file>
    snprintf(buf, bufsz, "/sys/class/net/%s/%s", ifname, file);
}

static int find_default_interface(char *out, size_t outsz) {
    // why: picking interface from default route is more robust than hardcoding
    FILE *f = fopen(DEFAULT_ROUTE_FILE, "r");
    if (!f) return -1;
    char line[256];
    // skip header
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    while (fgets(line, sizeof(line), f)) {
        // Fields: Iface Destination Gateway Flags RefCnt Use Metric Mask ...
        // Default route has Destination == 00000000
        char iface[64];
        char destination[64];
        if (sscanf(line, "%63s %63s", iface, destination) == 2) {
            if (strcmp(destination, "00000000") == 0) {
                strncpy(out, iface, outsz - 1);
                out[outsz - 1] = '\0';
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return -1;
}

static void choose_fallback_interface(char *out, size_t outsz) {
    const char *candidates[] = {"eth0", "ens33", "enp0s3", "wlan0", "lo"};
    for (size_t i = 0; i < sizeof(candidates)/sizeof(candidates[0]); ++i) {
        char path[256];
        build_if_path(path, sizeof(path), candidates[i], "operstate");
        FILE *f = fopen(path, "r");
        if (f) { fclose(f); strncpy(out, candidates[i], outsz - 1); out[outsz-1] = '\0'; return; }
    }
    // last resort
    strncpy(out, "lo", outsz - 1);
    out[outsz - 1] = '\0';
}

static void ensure_initialized(void) {
    if (g_inited) return;
    memset(&g_inst, 0, sizeof(g_inst));

    // Bind methods
    g_inst.get_tx_rx_info = get_tx_rx_info;
    g_inst.get_network_speed = get_network_speed;
    g_inst.get_bandwidth_usage = get_bandwidth_usage;
    g_inst.get_packet_info = get_packet_info;
    g_inst.get_connection_info = get_connection_info;
    g_inst.get_network_interface_info = get_network_interface_info;

    // Resolve primary interface
    if (find_default_interface(g_inst.primary_ifname, sizeof(g_inst.primary_ifname)) != 0) {
        choose_fallback_interface(g_inst.primary_ifname, sizeof(g_inst.primary_ifname));
    }

    g_inited = 1;
}

// --- Singleton API ---
Network_info *network_info_instance(void) {
    ensure_initialized();
    return &g_inst;
}

int network_set_interface(const char *ifname) {
    ensure_initialized();
    if (!ifname || !*ifname) return -1;
    // quick validate existence
    char path[256];
    build_if_path(path, sizeof(path), ifname, "operstate");
    FILE *f = fopen(path, "r");
    if (!f) return -2; // why: avoid storing non-existent interface name
    fclose(f);
    strncpy(g_inst.primary_ifname, ifname, sizeof(g_inst.primary_ifname) - 1);
    g_inst.primary_ifname[sizeof(g_inst.primary_ifname) - 1] = '\0';
    return 0;
}

const char *network_get_interface(void) {
    ensure_initialized();
    return g_inst.primary_ifname;
}

// --- Implementations ---
void get_tx_rx_info(Network_info *network_info) {
    ensure_initialized();
    (void)network_info; // singleton state stored in g_inst
    char path[256];
    uint64_t v = 0;

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "rx_bytes");
    if (read_uint64_from_file(path, &v) == 0) g_inst.speed_info.rx_bytes = v; else perror(path);

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "tx_bytes");
    if (read_uint64_from_file(path, &v) == 0) g_inst.speed_info.tx_bytes = v; else perror(path);
}

void get_network_speed(Network_info *network_info) {
    ensure_initialized();
    (void)network_info;

    struct timespec t0, t1;
    get_tx_rx_info(&g_inst);
    uint64_t tx0 = g_inst.speed_info.tx_bytes;
    uint64_t rx0 = g_inst.speed_info.rx_bytes;

    if (clock_gettime(CLOCK_MONOTONIC, &t0) != 0) memset(&t0, 0, sizeof(t0));
    sleep(1);
    get_tx_rx_info(&g_inst);
    if (clock_gettime(CLOCK_MONOTONIC, &t1) != 0) memset(&t1, 0, sizeof(t1));

    double dt = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)/1e9;
    if (dt <= 0) dt = 1.0; // why: avoid divide-by-zero if clock fails

    g_inst.speed_info.upload_speed_kBps = (double)(g_inst.speed_info.tx_bytes - tx0) / dt / 1024.0;
    g_inst.speed_info.download_speed_kBps = (double)(g_inst.speed_info.rx_bytes - rx0) / dt / 1024.0;
}

void get_bandwidth_usage(Network_info *network_info) {
    ensure_initialized();
    (void)network_info;

    // /sys/class/net/<if>/speed returns Mbps
    char path[256];
    build_if_path(path, sizeof(path), g_inst.primary_ifname, "speed");
    uint32_t mbps = 0;
    if (read_uint32_from_file(path, &mbps) != 0 || mbps == 0U) {
        // Unknown link speed -> mark usage as 0, but keep values measurable
        g_inst.bandwidth_usage_tx = 0.0;
        g_inst.bandwidth_usage_rx = 0.0;
        return;
    }
    // Convert KB/s to Mb/s: KB/s * 8 / 1024
    double up_mbps = g_inst.speed_info.upload_speed_kBps * 8.0 / 1024.0;
    double down_mbps = g_inst.speed_info.download_speed_kBps * 8.0 / 1024.0;
    g_inst.bandwidth_usage_tx = up_mbps / (double)mbps;
    g_inst.bandwidth_usage_rx = down_mbps / (double)mbps;
}

void get_packet_info(Network_info *network_info) {
    ensure_initialized();
    (void)network_info;
    char path[256];
    uint64_t v = 0;

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "rx_packets");
    if (read_uint64_from_file(path, &v) == 0) g_inst.packet_info.packet_rx = v; else perror(path);

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "tx_packets");
    if (read_uint64_from_file(path, &v) == 0) g_inst.packet_info.packet_tx = v; else perror(path);

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "rx_errors");
    if (read_uint64_from_file(path, &v) == 0) g_inst.packet_info.rx_errors = v; else perror(path);

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "tx_errors");
    if (read_uint64_from_file(path, &v) == 0) g_inst.packet_info.tx_errors = v; else perror(path);

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "rx_dropped");
    if (read_uint64_from_file(path, &v) == 0) g_inst.packet_info.rx_dropped = v; else perror(path);

    build_stat_path(path, sizeof(path), g_inst.primary_ifname, "tx_dropped");
    if (read_uint64_from_file(path, &v) == 0) g_inst.packet_info.tx_dropped = v; else perror(path);
}

#define TCP_ESTABLISHED "01"
#define UDP_LISTENING   "07"

static int count_states_in_proc(const char *path, const char *state_hex) {
    FILE *f = fopen(path, "r");
    if (!f) { perror(path); return -1; }
    char buf[512];
    int count = 0;
    if (!fgets(buf, sizeof(buf), f)) { fclose(f); return -1; }
    while (fgets(buf, sizeof(buf), f)) {
        char local[64], rem[64], st[8];
        // idx: sl local_address rem_address st ...
        if (sscanf(buf, "%*d: %63s %63s %2s", local, rem, st) == 3) {
            if (strncmp(st, state_hex, 2) == 0) count++;
        }
    }
    fclose(f);
    return count;
}

int get_connection_info(Network_info *network_info) {
    ensure_initialized();
    (void)network_info;
    int tcp = count_states_in_proc(PROC_TCP, TCP_ESTABLISHED);
    int udp = count_states_in_proc(PROC_UDP, UDP_LISTENING);
    if (tcp < 0) tcp = 0; if (udp < 0) udp = 0;
    g_inst.connection_info.total_tcp_connections = (uint32_t)tcp;
    g_inst.connection_info.total_udp_sockets = (uint32_t)udp;
    return tcp; // keep return similar to original (tcp count)
}

int get_network_interface_info(Network_info *network_info) {
    ensure_initialized();
    (void)network_info;

    struct ifaddrs *ifaddr = NULL, *ifa = NULL;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return -1;
    }
    int count = 0;
    for (ifa = ifaddr; ifa != NULL && count < NET_MAX_IFS; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
        struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
        const char *ip = inet_ntop(AF_INET, &(sa->sin_addr), g_inst.interface_info[count].ip_address, sizeof(g_inst.interface_info[count].ip_address));
        if (!ip) continue;
        strncpy(g_inst.interface_info[count].interface_name, ifa->ifa_name, sizeof(g_inst.interface_info[count].interface_name)-1);
        g_inst.interface_info[count].interface_name[sizeof(g_inst.interface_info[count].interface_name)-1] = '\0';
        count++;
    }
    freeifaddrs(ifaddr);
    return count;
}
