#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEMORY_TOP_PROCESSES 5

// Forward declaration for opaque pointer
typedef struct MemoryManager MemoryManager;

// --- Data Structures for public access ---
typedef struct {
    uint64_t pid;
    char name[64]; // Reduced size for efficiency
    double usage_percent;
} Process_mem_info;

// The main struct defining the module's data
// Users can read from this struct after getting a pointer from the facade
struct MemoryManager {
    // RAM
    uint64_t total_kb;
    uint64_t free_kb;
    uint64_t used_kb;
    // Swap
    uint64_t swap_total_kb;
    uint64_t swap_free_kb;
    uint64_t swap_used_kb;
    // Cache/Buffers
    uint64_t cached_kb;
    uint64_t buffers_kb;

    // Top processes by RAM
    Process_mem_info top_processes[MEMORY_TOP_PROCESSES];
    int top_process_count;

    // Internal function pointers (implementation detail)
    void (*update_stats)(void);
    void (*update_top_processes)(void);
};


// --- Singleton Management API ---

// NEW: Destructor to free the singleton instance
void memory_manager_destroy(void);

// MODIFIED: Renamed to create() for consistency
MemoryManager *memory_manager_create(void);


// --- Public Data Update API ---
void memory_update_stats(MemoryManager *manager);
void memory_update_top_processes(MemoryManager *manager);


#ifdef __cplusplus
}
#endif

#endif // MEMORY_H_