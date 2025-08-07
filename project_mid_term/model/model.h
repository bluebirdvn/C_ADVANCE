#ifndef _MODEL_H
#define _MODEL_H

#include "cpu.h"
#include "network.h"
#include "storage.h"
#include "system.h"
#include "memory.h"

typedef struct {
    Storage_info *storage_info;
    Network_info *network_info;
    Memory_info *memory_info;
    cpu_usage *cpu_usage;
}

#endif //_MODEL_H