#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <time.h>

#define DISKSTATS_PATH "/proc/diskstats"
#define DEFAULT_TESTFILE_NAME "storage_speed.bin"
#define DEFAULT_BLOCK_SIZE 4096
#define DEFAULT_FILE_SIZE_MB 10

struct Storage_info {
    // Config
    char mount_path[256];
    char device_name[64];
    char testfile_dir[256];
    size_t block_size;   // bytes
    size_t file_size_mb; // MB

    // Data
    Storage_capacity capacity;
    Storage_perf perf;
    Storage_sectors sectors;

    // Methods
    void (*get_storage_info)(Storage_info *);
    void (*get_speed_and_iops_info)(Storage_info *);
    void (*get_sectors_info)(Storage_info *);
};

static struct Storage_info Storage_info_instance;
static int g_inited = 0;

// --------- helpers ---------
static void ensure_init(void);

static int guess_first_nonloop_device(char *out, size_t outsz) {
    FILE *f = fopen(DISKSTATS_PATH, "r");
    if (!f) return -1;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        // fields: major minor name ...
        char name[64];
        if (sscanf(line, "%*u %*u %63s", name) == 1) {
            if (strncmp(name, "loop", 4) != 0 && strncmp(name, "ram", 3) != 0 && strlen(name) > 0) {
                strncpy(out, name, outsz - 1);
                out[outsz - 1] = '\0';
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return -1;
}

static void pick_defaults(void) {
    if (g_si.mount_path[0] == '\0') {
        strncpy(g_si.mount_path, "/", sizeof(g_si.mount_path) - 1);
    }
    if (g_si.testfile_dir[0] == '\0') {
        const char *tmp = getenv("TMPDIR");
        strncpy(g_si.testfile_dir, tmp && *tmp ? tmp : "/tmp", sizeof(g_si.testfile_dir) - 1);
    }
    if (g_si.device_name[0] == '\0') {
        char dev[64] = {0};
        if (guess_first_nonloop_device(dev, sizeof(dev)) == 0) {
            strncpy(g_si.device_name, dev, sizeof(g_si.device_name) - 1);
        } else {
            strncpy(g_si.device_name, "sda", sizeof(g_si.device_name) - 1);
        }
    }
    if (g_si.block_size == 0) g_si.block_size = DEFAULT_BLOCK_SIZE;
    if (g_si.file_size_mb == 0) g_si.file_size_mb = DEFAULT_FILE_SIZE_MB;
}

static void ensure_init(void) {
    if (g_inited) return;
    memset(&Storage_info_instance, 0, sizeof(Storage_info_instance));
    Storage_info_instance.get_storage_info = get_storage_info;
    Storage_info_instance.get_speed_and_iops_info = get_speed_and_iops_info;
    Storage_info_instance.get_sectors_info = get_sectors_info;
    pick_defaults();
    g_inited = 1;
}

// ---- public setters ----
Storage_info *storage_info_instance(void) { ensure_init(); return &Storage_info_instance; }

int storage_set_mount_path(const char *path) {
    ensure_init();
    if (!path || !*path) return -1;
    strncpy(g_si.mount_path, path, sizeof(g_si.mount_path) - 1);
    g_si.mount_path[sizeof(g_si.mount_path) - 1] = '\0';
    return 0;
}

int storage_set_device(const char *device) {
    ensure_init();
    if (!device || !*device) return -1;
    strncpy(g_si.device_name, device, sizeof(g_si.device_name) - 1);
    g_si.device_name[sizeof(g_si.device_name) - 1] = '\0';
    return 0;
}

int storage_set_testfile_dir(const char *dir) {
    ensure_init();
    if (!dir || !*dir) return -1;
    strncpy(g_si.testfile_dir, dir, sizeof(g_si.testfile_dir) - 1);
    g_si.testfile_dir[sizeof(g_si.testfile_dir) - 1] = '\0';
    return 0;
}

// ------- I/O helpers -------
static void join_path(char *out, size_t outsz, const char *a, const char *b) {
    size_t la = strlen(a);
    int need_slash = (la > 0 && a[la-1] != '/');
    snprintf(out, outsz, need_slash ? "%s/%s" : "%s%s", a, b);
}

static int read_diskstats_bytes(const char *device, uint64_t *bytes_read, uint64_t *bytes_written) {
    FILE *f = fopen(DISKSTATS_PATH, "r");
    if (!f) return -1;
    char line[512];
    int ok = 0;
    while (fgets(line, sizeof(line), f)) {
        char name[64];
        unsigned long long rd_sectors = 0ULL, wr_sectors = 0ULL;
        // columns per Linux kernel docs; we pick fields: reads_merged is ignored, we use sectors read/written
        // Example parse positions may vary by kernel; robustly scan pulling two numbers near tail using %* to skip
        // Format (3.10+): major minor name reads completed ... sectors_read ... writes completed ... sectors_written ...
        // We'll use a broad sscanf with many * to skip fields.
        if (sscanf(line, "%*u %*u %63s %*u %*u %*u %llu %*u %*u %*u %llu",
                   name, &rd_sectors, &wr_sectors) == 3) {
            if (strcmp(name, device) == 0) {
                if (bytes_read) *bytes_read = rd_sectors * 512ULL;
                if (bytes_written) *bytes_written = wr_sectors * 512ULL;
                ok = 1;
                break;
            }
        }
    }
    fclose(f);
    return ok ? 0 : -2;
}

// -------- methods impl --------
void get_storage_info(Storage_info *storage_info) {
    (void)storage_info; // singleton state only
    ensure_init();

    struct statvfs st;
    if (statvfs(g_si.mount_path, &st) != 0) {
        perror("statvfs");
        g_si.capacity.total_bytes = g_si.capacity.used_bytes = g_si.capacity.free_bytes = 0;
        return;
    }
    unsigned long long block_size = (unsigned long long)st.f_frsize;
    unsigned long long total = (unsigned long long)st.f_blocks * block_size;
    unsigned long long freeb = (unsigned long long)st.f_bavail * block_size; // why: b_avail excludes reserved blocks for root

    g_si.capacity.total_bytes = (uint64_t)total;
    g_si.capacity.free_bytes = (uint64_t)freeb;
    g_si.capacity.used_bytes = (uint64_t)(total - freeb);
}

void get_speed_and_iops_info(Storage_info *storage_info) {
    (void)storage_info;
    ensure_init();

    char filepath[512];
    join_path(filepath, sizeof(filepath), g_si.testfile_dir, DEFAULT_TESTFILE_NAME);

    FILE *fp = fopen(filepath, "wb");
    if (!fp) { perror("open test file for write"); return; }

    size_t blk = g_si.block_size;
    size_t total_bytes = g_si.file_size_mb * 1024UL * 1024UL;
    char *buf = (char*)malloc(blk);
    if (!buf) { fclose(fp); fprintf(stderr, "malloc failed\n"); return; }
    memset(buf, 0, blk);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (size_t written = 0; written < total_bytes; written += blk) {
        size_t w = fwrite(buf, 1, blk, fp);
        if (w != blk) { perror("fwrite"); break; }
    }
    fflush(fp);
    fsync(fileno(fp)); // why: force flush to measure device, not page cache
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fclose(fp);

    double dtw = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)/1e9;
    if (dtw <= 0) dtw = 1.0;
    g_si.perf.write_MBps = ((double)total_bytes / (1024.0*1024.0)) / dtw;
    g_si.perf.iops_write = ((double)total_bytes / (double)blk) / dtw;

    // READ phase
    fp = fopen(filepath, "rb");
    if (!fp) { perror("open test file for read"); free(buf); return; }
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (size_t read = 0; read < total_bytes; read += blk) {
        size_t r = fread(buf, 1, blk, fp);
        if (r != blk) { if (feof(fp)) break; perror("fread"); break; }
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fclose(fp);

    double dtr = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)/1e9;
    if (dtr <= 0) dtr = 1.0;
    g_si.perf.read_MBps = ((double)total_bytes / (1024.0*1024.0)) / dtr;
    g_si.perf.iops_read = ((double)total_bytes / (double)blk) / dtr;

    // cleanup
    remove(filepath);
    free(buf);
}

void get_sectors_info(Storage_info *storage_info) {
    (void)storage_info;
    ensure_init();

    uint64_t br = 0, bw = 0;
    if (read_diskstats_bytes(g_si.device_name, &br, &bw) != 0) {
        // keep zeros if not found
        return;
    }
    g_si.sectors.bytes_read = br;
    g_si.sectors.bytes_written = bw;
}
