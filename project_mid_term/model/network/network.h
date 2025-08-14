#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_IPV4_PER_IF 4
#define MAX_IPV6_PER_IF 4

typedef struct NetworkManager NetworkManager;

struct NetworkManager{
    char interface_name[32];

    // IP Addresses
    char ipv4_addrs[MAX_IPV4_PER_IF][16];   
    int  ipv4_count;
    char ipv6_addrs[MAX_IPV6_PER_IF][46];   
    int  ipv6_count;

    // Cumulative Stats
    uint64_t rx_bytes, tx_bytes;
    uint64_t rx_packets, tx_packets;

    // Speeds
    double rx_speed_kbps, tx_speed_kbps;
    
    // Connections
    int tcp_connections_total;
    int tcp_connections_established;

    // Interface Info
    int link_up;

    // --- Internal fields ---
    uint64_t prev_rx_bytes, prev_tx_bytes;
    struct timespec last_ts;
    void (*update_bytes)(void);
    void (*update_speed)(void);
    void (*update_connections)(void);
    void (*update_interface_info)(void);
};

// --- Singleton Management API ---
void network_manager_destroy(void);
NetworkManager *network_manager_create(void);

// --- Public Data Update API ---
void get_bytes(NetworkManager *manager);
void get_speed(NetworkManager *manager);
void get_connections(NetworkManager *manager);
void get_interface_info(NetworkManager *manager);

#ifdef __cplusplus
}
#endif

#endif // NETWORK_H_