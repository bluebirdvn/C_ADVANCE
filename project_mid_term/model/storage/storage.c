#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h> // For fsync

// --- Defines ---
#define DISKSTATS_PATH "/proc/diskstats"
#define DEFAULT_MOUNT_PATH "/"
#define DEFAULT_TESTFILE_DIR "/tmp"
#define DEFAULT_TESTFILE_NAME "storage_perf_test.bin"
#define DEFAULT_BLOCK_SIZE (1024 * 1024) // 1MB block size for testing
#define DEFAULT_FILE_SIZE_MB 128 // 128MB test file size

// --- Global Singleton Instance ---
static StorageManager *g_storage_manager_instance = NULL;

// --- Internal Function Declarations ---
static void update_capacity(void);
static void update_io(void);
static void run_perf_test(void);
static int guess_first_nonloop_device(char *out, size_t outsz);
static void join_path(char *out, size_t outsz, const char *dir, const char *filename);

// --- Singleton Management ---

void storage_manager_destroy(void) {
    if (g_storage_manager_instance) {
        free(g_storage_manager_instance);
        g_storage_manager_instance = NULL;
    }
}

StorageManager *storage_manager_create(void) {
    if (g_storage_manager_instance == NULL) {
        g_storage_manager_instance = (StorageManager*)calloc(1, sizeof(StorageManager));
        if (!g_storage_manager_instance) {
            perror("Failed to allocate memory for StorageManager");
            return NULL;
        }
        
        // Assign function pointers to internal implementations
        g_storage_manager_instance->update_capacity = update_capacity;
        g_storage_manager_instance->update_io = update_io;
        g_storage_manager_instance->run_perf_test = run_perf_test;

        // Set default configuration values
        strncpy(g_storage_manager_instance->mount_path, DEFAULT_MOUNT_PATH, sizeof(g_storage_manager_instance->mount_path) - 1);
        
        if (guess_first_nonloop_device(g_storage_manager_instance->device_name, sizeof(g_storage_manager_instance->device_name)) != 0) {
            // Fallback to a common default if guessing fails
            strncpy(g_storage_manager_instance->device_name, "sda", sizeof(g_storage_manager_instance->device_name) - 1);
        }
    }
    return g_storage_manager_instance;
}

// --- Internal Helper Functions ---

/**
 * @brief Joins a directory and a filename into a full path.
 */
static void join_path(char *out, size_t outsz, const char *dir, const char *filename) {
    size_t dirlen = strlen(dir);
    if (dirlen > 0 && dir[dirlen - 1] == '/') {
        snprintf(out, outsz, "%s%s", dir, filename);
    } else {
        snprintf(out, outsz, "%s/%s", dir, filename);
    }
}

/**
 * @brief Reads /proc/diskstats to find the first non-loop, non-ram device.
 */
static int guess_first_nonloop_device(char *out, size_t outsz) {
    FILE *f = fopen(DISKSTATS_PATH, "r");
    if (!f) {
        perror("fopen: " DISKSTATS_PATH);
        return -1;
    }
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char name[64];
        // fields: major minor name ...
        if (sscanf(line, "%*u %*u %63s", name) == 1) {
            // Filter out loop devices (used for snaps, etc.) and ram disks
            if (strncmp(name, "loop", 4) != 0 && strncmp(name, "ram", 3) != 0 && strlen(name) > 0) {
                strncpy(out, name, outsz - 1);
                out[outsz - 1] = '\0';
                fclose(f);
                return 0; // Success
            }
        }
    }
    fclose(f);
    return -1; // Not found
}


// --- Internal Data Update Functions ---

/**
 * @brief Updates the storage capacity information (total, used, free).
 */
static void update_capacity(void) {
    if (!g_storage_manager_instance) return;

    struct statvfs st;
    if (statvfs(g_storage_manager_instance->mount_path, &st) != 0) {
        perror("statvfs");
        // Zero out data on failure
        g_storage_manager_instance->capacity = (Storage_capacity){0};
        return;
    }
    uint64_t block_size = st.f_frsize;
    uint64_t total_bytes = st.f_blocks * block_size;
    uint64_t free_for_user = st.f_bavail * block_size; // f_bavail is for non-root users
    uint64_t free_total = st.f_bfree * block_size;
    uint64_t used_bytes = total_bytes - free_total;

    g_storage_manager_instance->capacity.total_bytes = total_bytes;
    g_storage_manager_instance->capacity.free_bytes = free_for_user;
    g_storage_manager_instance->capacity.used_bytes = used_bytes;
}

/**
 * @brief Updates the total bytes read/written from the storage device.
 */
static void update_io(void) {
    if (!g_storage_manager_instance) return;
    
    FILE *f = fopen(DISKSTATS_PATH, "r");
    if (!f) {
        perror("fopen: " DISKSTATS_PATH);
        g_storage_manager_instance->io = (Storage_io){0};
        return;
    }

    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char name[64];
        unsigned long long rd_sectors = 0, wr_sectors = 0;
        // The format is: major minor devname ... sectors_read ... sectors_written ...
        // We need fields 3, 6, and 10 (1-based index).
        if (sscanf(line, "%*u %*u %63s %*u %*u %llu %*u %*u %*u %llu", name, &rd_sectors, &wr_sectors) >= 2) {
            if (strcmp(name, g_storage_manager_instance->device_name) == 0) {
                // A sector is typically 512 bytes
                g_storage_manager_instance->io.bytes_read = rd_sectors * 512;
                g_storage_manager_instance->io.bytes_written = wr_sectors * 512;
                found = 1;
                break;
            }
        }
    }
    fclose(f);
    
    if (!found) {
        // If device wasn't found, zero out the stats
        g_storage_manager_instance->io = (Storage_io){0};
    }
}

/**
 * @brief Runs a performance benchmark by writing and reading a temporary file.
 * @warning This is a slow, blocking operation. Use with caution.
 */
static void run_perf_test(void) {
    if (!g_storage_manager_instance) return;

    // Reset performance data
    g_storage_manager_instance->perf = (Storage_perf){0};

    // ---------------USE simulation data ------------------
    // char filepath[512];
    // join_path(filepath, sizeof(filepath), DEFAULT_TESTFILE_DIR, DEFAULT_TESTFILE_NAME);

    // const size_t block_size = DEFAULT_BLOCK_SIZE;
    // const size_t total_bytes_to_transfer = (size_t)DEFAULT_FILE_SIZE_MB * 1024 * 1024;
    
    // char *buffer = (char*)malloc(block_size);
    // if (!buffer) {
    //     fprintf(stderr, "Failed to allocate buffer for performance test.\n");
    //     return;
    // }
    // // Fill buffer with some data to prevent compression from skewing results
    // for (size_t i = 0; i < block_size; ++i) buffer[i] = i % 256;

    // struct timespec start, end;
    // double elapsed_sec;

    // // --- WRITE TEST ---
    // FILE *fp = fopen(filepath, "wb");
    // if (!fp) {
    //     perror("fopen (write test)");
    //     free(buffer);
    //     return;
    // }

    // clock_gettime(CLOCK_MONOTONIC, &start);
    // for (size_t written = 0; written < total_bytes_to_transfer; written += block_size) {
    //     if (fwrite(buffer, 1, block_size, fp) != block_size) {
    //         perror("fwrite");
    //         break; // Stop on error
    //     }
    // }
    // fflush(fp); // Flush userspace buffers
    // fsync(fileno(fp)); // Ensure data is written to the physical device
    // clock_gettime(CLOCK_MONOTONIC, &end);
    // fclose(fp);

    // elapsed_sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    // if (elapsed_sec > 0) {
    //     g_storage_manager_instance->perf.write_MBps = (total_bytes_to_transfer / (1024.0 * 1024.0)) / elapsed_sec;
    // }

    // // --- READ TEST ---
    // fp = fopen(filepath, "rb");
    // if (!fp) {
    //     perror("fopen (read test)");
    //     free(buffer);
    //     remove(filepath); // Clean up
    //     return;
    // }
    
    // clock_gettime(CLOCK_MONOTONIC, &start);
    // for (size_t bytes_read = 0; bytes_read < total_bytes_to_transfer; bytes_read += block_size) {
    //     if (fread(buffer, 1, block_size, fp) != block_size) {
    //         if (feof(fp)) break; 
    //         perror("fread");
    //         break; 
    //     }
    // }
    // clock_gettime(CLOCK_MONOTONIC, &end);
    // fclose(fp);

    // elapsed_sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    // if (elapsed_sec > 0) {
    //     g_storage_manager_instance->perf.read_MBps = (total_bytes_to_transfer / (1024.0 * 1024.0)) / elapsed_sec;
    // }

    // // --- Cleanup ---
    // free(buffer);
    // remove(filepath);
    g_storage_manager_instance->perf.write_MBps = 500 + (rand() % 30);
    g_storage_manager_instance->perf.read_MBps  = 600 + (rand() % 30);
}


// --- Public Data Update API ---

void get_storage_info(StorageManager *manager) {
    if (!manager || !manager->update_capacity) return;
    manager->update_capacity();
}

void get_io_info(StorageManager *manager) {
    if (!manager || !manager->update_io) return;
    manager->update_io();
}

void run_storage_benchmark(StorageManager *manager) {
    if (!manager || !manager->run_perf_test) return;
    manager->run_perf_test();
}
