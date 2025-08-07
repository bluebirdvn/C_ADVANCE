#ifndef _NETWORK_H
#define _NETWORK_H

typedef struct {
    uint64_t tx_bytes;
    uint64_t rx_bytes;
} Network_tx_rx_info;

typedef struct {
    Network_tx_rx_info tx_rx_info[2];
    uint64_t upload_speed;
    uint64_t download_speed;
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
    uint32_t total_tcp_connections;
    uint32_t total_udp_connections;
} Network_connection_info;


typedef struct {
    char interface_name[64];
    char ip_address[16];
} Network_interface_info;


typedef struct {
    Network_speed_info speed_info;
    uint64_t bandwidth_usage;
    uint64_t latency;
    Network_packet_info packet_info;
    Network_connection_info connection_info;
    Network_interface_info interface_info[10];
} Network_info;


/*
*@function: get_tx_rx_info
*@description: Retrieves the transmitted and received bytes for network interfaces.
*@param network_info: Pointer to Network_info structure to store the retrieved data.
*/
void get_tx_rx_info(Network_info *network_info);


/*
*@function: get_network_speed
*@description: Calculates the upload and download speeds of the network interfaces through the result of get_tx_rx_info.
*@param network_info: Pointer to Network_info structure to store the calculated speeds.
*@note: This function should be called after or within get_tx_rx_info.
*/
void get_network_speed(Network_info *network_info);

/*
* @function: get_bandwidth_usage
* @description: Get bandwidth usage of the network interfaces.
* @param: network_info: Pointer to Network_info structure to store the bandwidth usage.
*/
void get_bandwidth_usage(Network_info *network_info);

/*
* @function: get_packet_info
* @description: Get packet information for the network interfaces.
* @param: network_info: Pointer to Network_info structure to store the packet information.
*/
void get_packet_info(Network_info *network_info);

/*
* @function: get_connection_info
* @description: Get connection information for the network interfaces.
* @param: network_info: Pointer to Network_info structure to store the connection information.
*/
void get_connection_info(Network_info *network_info);
/*
* @function: get_network_interface_info
* @description: this function retrieves information about the network interfaces.
* @param: network_info: Pointer to Network_info structure to store the network interface information.
*/
void get_network_interface_info(Network_info *network_info);

#endif // _NETWORK_H