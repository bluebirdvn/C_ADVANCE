#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StorageManager StorageManager;

// --- Public Data Structures ---
typedef struct {
    uint64_t total_bytes;
    uint64_t used_bytes;
    uint64_t free_bytes;
} Storage_capacity;

typedef struct {
    double read_MBps;
    double write_MBps;
} Storage_perf;

typedef struct {
    uint64_t bytes_read;
    uint64_t bytes_written;
} Storage_io;

struct StorageManager {
    // Config
    char mount_path[256];
    char device_name[64];
    
    // Data
    Storage_capacity capacity;
    Storage_perf perf;
    Storage_io io;

    // --- Internal ---
    void (*update_capacity)(void);
    void (*update_io)(void);
    void (*run_perf_test)(void); // Renamed for clarity
};


// --- Singleton Management API ---
void storage_manager_destroy(void);
StorageManager *storage_manager_create(void);

// --- Public Data Update API ---
void get_storage_info(StorageManager *manager);
void get_io_info(StorageManager *manager);
void run_storage_benchmark(StorageManager *manager); // Renamed

#ifdef __cplusplus
}
#endif

#endif // STORAGE_H_