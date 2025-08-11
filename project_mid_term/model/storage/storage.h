#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Storage_info Storage_info; // opaque

// Data containers
typedef struct {
    uint64_t total_bytes;
    uint64_t used_bytes;
    uint64_t free_bytes;
} Storage_capacity;

typedef struct {
    double read_MBps;
    double write_MBps;
    double iops_read;   // ops/s
    double iops_write;  // ops/s
} Storage_perf;

typedef struct {
    uint64_t bytes_read;    // from sectors * 512
    uint64_t bytes_written; // from sectors * 512
} Storage_sectors;

// Singleton accessors
Storage_info *storage_info_instance(void);
int storage_set_mount_path(const char *path);     // default: "/"
int storage_set_device(const char *device);       // default: best-guess from /proc/diskstats (first non-loop)
int storage_set_testfile_dir(const char *dir);    // default: $TMPDIR or "/tmp"

// Public API (standalone functions + also bound to function pointers inside struct)
void get_storage_info(Storage_info *storage_info);
void get_speed_and_iops_info(Storage_info *storage_info);
void get_sectors_info(Storage_info *storage_info);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_H_