
#include "storage.h"

#define PATH "/"
#define FILE_SIZE_MB 10
#define BLOCK_SIZE 4096

void get_storage_info(Storage_info *storage_info)
{
    struct statvfs stat;
    if (statvfs(PATH, &stat) != 0) {
        perror("Failed to get storage info");
        return;
    }
    storage_info->total_storage = stat.f_blocks * stat.f_frsize;
    storage_info->free_storage = stat.f_bsize  * stat.f_bavail;
    storage_info->used_storage = storage_info->total_storage - storage_info->free_storage;
}

void get_speed_and_iops_info(Storage_info *storage_info)
{
    FILE *fp = fopen("/home/shunkun/storage_speed.bin", "wb");
    if (!fp) {
        perror("Failed to open storage_speed.bin file.");
        return;
    }

    char *buffer = malloc(BLOCK_SIZE);
    memset(buffer, 0, BLOCK_SIZE);

    size_t total_size = FILE_SIZE_MB * 1024 * 1024;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t written = 0; written < total_size; written += BLOCK_SIZE) {
        fwrite(buffer, 1, BLOCK_SIZE, fp);
    }

    fflush(fp);
    fsync(fileno(fp));

    fclose(fp);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double slapsed_time = (end.tv_nsec - start.tv_nsec) / 1e9 + (end.tv_sec - start.tv_sec);

    storage_info->write_speed = (double)total_size / slapsed_time / (1024 * 1024);
    storage_info->iops_written = total_size / BLOCK_SIZE / slapsed_time;

    fp = fopen("/home/shunkun/storage_speed.bin", "rb");
    if (!fp) {
        perror("Failed to open storage_speed.bin file for reading.");
        free(buffer);
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t read = 0; read < total_size; read += BLOCK_SIZE) {
        fread(buffer, 1, BLOCK_SIZE, fp);
    }

    fflush(fp);
    fsync(fileno(fp));
    clock_gettime(CLOCK_MONOTONIC, &end);

    slapsed_time = (end.tv_nsec - start.tv_nsec) / 1e9 + (end.tv_sec - start.tv_sec);

    storage_info->read_speed = (double)total_size / slapsed_time / (1024 * 1024);
    storage_info->iops_read = total_size / BLOCK_SIZE / slapsed_time;

    fclose(fp);
    free(buffer);
}

void get_sectors_info(Storage_info *storage_info) 
{
    FILE *fp = fopen("/proc/diskstats", "r");
    if (!fp) {
        perror("Failed to open /proc/diskstats");
        return;
    }
    char device[32];
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        sscanf(buffer, "%*d %*d %s", device);
        if (strcmp(device, "sda") == 0) {
                sscanf(buffer, "%*d %*d %*s %*u %*u %lu %*u %*u %*u %lu",
               &storage_info->sectors_read, &storage_info->sectors_written);
        break;
    }
    }
    fclose(fp);

    storage_info->sectors_read *= 512; // Convert sectors to bytes
    storage_info->sectors_written *= 512; // Convert sectors to bytes
}


int main(void) {
    Storage_info storage_info;
    get_storage_info(&storage_info);
    printf("Total Storage: %lu GB\n", storage_info.total_storage/1024/1024/1024);
    printf("Free Storage: %lu GB\n", storage_info.free_storage/1024/1024/1024);
    printf("Used Storage: %lu GB\n", storage_info.used_storage/1024/1024/1024);

    get_speed_and_iops_info(&storage_info);
    printf("Read Speed: %.2f MB/s\n", storage_info.read_speed);
    printf("Write Speed: %.2f MB/s\n", storage_info.write_speed);
    printf("IOPS Read: %lu\n", storage_info.iops_read);
    printf("IOPS Written: %lu\n", storage_info.iops_written);

    get_sectors_info(&storage_info);
    printf("Sectors Read: %lu sectors\n", storage_info.sectors_read/512);
    printf("Sectors Written: %lu sectors\n", storage_info.sectors_written/512);

    return 0;
}