#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Forward opaque type ---
typedef struct Network_info Network_info;

// --- Data structures ---
typedef struct {
    uint64_t tx_bytes;
    uint64_t rx_bytes;
    double upload_speed_kBps;   // KB/s
    double download_speed_kBps;  // KB/s
} Network_speed_info;

typedef struct {
    uint64_t packet_tx;
    uint64_t packet_rx;
    uint64_t rx_errors;
    uint64_t tx_errors;
    uint64_t rx_dropped;
    uint64_t tx_dropped;
} Network_packet_info;

typedef struct {
    uint32_t total_tcp_connections; // ESTABLISHED
    uint32_t total_udp_sockets;     // UDP in use (close to '07' state)
} Network_connection_info;

typedef struct {
    char interface_name[64];
    char ip_address[16]; // IPv4 string
} Network_interface_info;

// --- Singleton Accessors ---
Network_info *network_info_instance(void);
int network_set_interface(const char *ifname);
const char *network_get_interface(void);

// --- Public API (standalone functions mirroring methods) ---
void get_tx_rx_info(Network_info *network_info);
void get_network_speed(Network_info *network_info);
void get_bandwidth_usage(Network_info *network_info);
void get_packet_info(Network_info *network_info);
int  get_connection_info(Network_info *network_info);
int  get_network_interface_info(Network_info *network_info);

#ifdef __cplusplus
}
#endif

#endif // NETWORK_H_