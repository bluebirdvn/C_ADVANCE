#include "network.h"

void get_tx_rx_info(Network_info *network_info)
{
    if (network_info == NULL) {
        fprintf(stderr, "Invalid network_info pointer\n");
        return;
    }

    FILE *fp_rx = fopen("/sys/class/net/ens33/statistics/rx_bytes", "r");
    if (fp_rx == NULL) {
        perror("Can't open /sys/class/net/ens33/statistics/rx_bytes");
        return;
    }

    FILE *fp_tx = fopen("/sys/class/net/ens33/statistics/tx_bytes", "r");
    if (fp_tx == NULL) {
        perror("Can't open /sys/class/net/ens33/statistics/tx_bytes");
        fclose(fp_rx);
        return;
    }

    fscanf(fp_rx, "%lu", &network_info->speed_info.rx_bytes);
    fscanf(fp_tx, "%lu", &network_info->speed_info.tx_bytes);

    fclose(fp_rx);
    fclose(fp_tx);
    
}

void get_network_speed(Network_info *network_info)
{
    if (network_info == NULL) {
        fprintf(stderr, "Invalid network_info pointer\n");
        return;        
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    get_tx_rx_info(network_info);

    uint64_t tx = network_info->speed_info.tx_bytes;
    uint64_t rx = network_info->speed_info.rx_bytes;

    sleep(1);

    get_tx_rx_info(network_info);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_time = (end.tv_nsec - start.tv_nsec) / 1e9 + (end.tv_sec - start.tv_sec);

    network_info->speed_info.upload_speed = (double)(network_info->speed_info.tx_bytes - tx) / elapsed_time / 1024;
    network_info->speed_info.download_speed = (double)(network_info->speed_info.rx_bytes - rx) / elapsed_time / 1024;
}

void get_bandwidth_usage(Network_info *network_info)
{
    if (network_info == NULL) {
        fprintf(stderr, "Invalid network_info pointer\n");
        return;        
    }
    FILE *fp = fopen("/sys/class/net/ens33/speed", "r");
    if (!fp) {
        perror("Can't open /sys/class/net/ens33/speed");
        return;      
    }

    uint32_t total_bandwidth = 0;

    fscanf(fp, "%u", &total_bandwidth);

    network_info->bandwidth_usage[0] = (double)(network_info->speed_info.upload_speed) / total_bandwidth;
    network_info->bandwidth_usage[1] = (double)(network_info->speed_info.download_speed) / total_bandwidth;    
    
}


void get_packet_info(Network_info *network_info)
{
    if (network_info == NULL) {
        fprintf(stderr, "Invalid network_info pointer\n");
        return;        
    }
    FILE *fp_rx = fopen("/sys/class/net/ens33/statistics/rx_packets", "r");
    if (!fp_rx) {
        perror("Can't open /sys/class/net/ens33/statistics/rx_packets");
        return;      
    }
    fscanf(fp_rx, "%lu", &network_info->packet_info.packet_rx);
    fclose(fp_rx);

    FILE *fp_tx = fopen("/sys/class/net/ens33/statistics/tx_packets", "r");
    if (!fp_tx) {
        perror("Can't open /sys/class/net/ens33/statistics/rx_packets");
        return;      
    }
    fscanf(fp_tx, "%lu", &network_info->packet_info.packet_tx);
    fclose(fp_tx);

    FILE *fp_tx_dropped = fopen("/sys/class/net/ens33/statistics/tx_dropped", "r");
    if (!fp_tx_dropped) {
        perror("Can't open /sys/class/net/ens33/statistics/tx_dropped");
        return;      
    }
    fscanf(fp_tx_dropped, "%lu", &network_info->packet_info.tx_dropped);
    fclose(fp_tx_dropped);

    FILE *fp_rx_dropped = fopen("/sys/class/net/ens33/statistics/rx_dropped", "r");
    if (!fp_rx_dropped) {
        perror("Can't open /sys/class/net/ens33/statistics/tx_dropped");
        return;      
    }
    fscanf(fp_rx_dropped, "%lu", &network_info->packet_info.rx_dropped);
    fclose(fp_rx_dropped);

    FILE *fp_rx_errors = fopen("/sys/class/net/ens33/statistics/rx_errors", "r");
    if (!fp_rx_errors) {
        perror("Can't open /sys/class/net/ens33/statistics/rx_errors");
        return;      
    }
    fscanf(fp_rx_errors, "%lu", &network_info->packet_info.rx_errors);
    fclose(fp_rx_errors);

    FILE *fp_tx_errors = fopen("/sys/class/net/ens33/statistics/tx_errors", "r");
    if (!fp_tx_errors) {
        perror("Can't open /sys/class/net/ens33/statistics/tx_errors");
        return;      
    }
    fscanf(fp_tx_errors, "%lu", &network_info->packet_info.tx_errors);
    fclose(fp_tx_errors);
}

#define TARGET_STATE "01"

int get_connection_info(Network_info *network_info) 
{
    if (network_info == NULL) {
        fprintf(stderr, "Invalid network_info pointer\n");
        return -1;         
    }

    FILE *f_tcp = fopen("/proc/net/tcp", "r");
    if (!f_tcp) {
        perror("Can't open /proc/net/tcp");
        return -1;
    }

    char buffer[256];

    fgets(buffer, sizeof(buffer), f_tcp);
    int num_connection = 0;
    while(fgets(buffer, sizeof(buffer), f_tcp)) {
        char local_addr[20];
        char rem_addr[20];
        char state[8];
        sscanf(buffer, "%*d: %20s %20s %2s", local_addr, rem_addr, state);

        if (strcmp(state, TARGET_STATE) == 0) {
            num_connection++;
        }
    }

    fclose(f_tcp);

    return num_connection;
}

int get_network_interface_info(Network_info *network_info)
{
    if (network_info == NULL) {
        fprintf(stderr, "Invalid network_info pointer\n");
        return -1;         
    }

    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return -1;
    }
    int count = 0;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {   
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
        inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
        strcpy(network_info->interface_info[count].interface_name, ifa->ifa_name);
        strcpy(network_info->interface_info[count].ip_address, ip);
        count++;
    }
    freeifaddrs(ifaddr);
    
    return count;
}



int main(void) 
{
    Network_info network;
    get_network_speed(&network);

    get_bandwidth_usage(&network);

    get_packet_info(&network);

    int count = get_network_interface_info(&network);

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            printf("Network_interface: %s | IP: %s\n", network.interface_info[i].interface_name, network.interface_info[i].ip_address);
        }
    }
    int num = get_connection_info(&network);
    if (num > 0) {
        printf("Now, device has %d connection.\n", num);
    }
    printf("Tx_bytes:%lu\nRx_bytes: %lu\nUp_speed: %lf Mb/s\nDown_speed: %lf Mb/s\n", network.speed_info.tx_bytes, network.speed_info.rx_bytes, network.speed_info.upload_speed, network.speed_info.download_speed);
    printf("Bandwidth usage: Tx: %.2lf %%, Rx: %.2lf %%", network.bandwidth_usage[0] * 100, network.bandwidth_usage[1] * 100);
    printf("Packets infor: \nTx: %lu\nRx: %lu\nErrors_Tx: %lu\nErrors_Rx: %lu\nDropped_Tx: %lu\nDropped_Rx: %lu\n",
                        network.packet_info.packet_tx,
                        network.packet_info.packet_rx, 
                        network.packet_info.tx_errors,
                        network.packet_info.rx_errors,
                        network.packet_info.tx_dropped,
                        network.packet_info.rx_dropped);
    return 0;
}