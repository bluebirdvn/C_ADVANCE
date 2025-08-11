#ifndef _MODEL_H
#define _MODEL_H

#include "cpu/cpu.h"
#include "network/network.h"
#include "storage/storage.h"
#include "system/system.h"
#include "memory/memory.h"

typedef struct System_monitor system_monitor;

struct System_monitor{
    Storage_info *storage_info;
    Network_info *network_info;
    Memory_info *memory_info;
    Cpu_usage *cpu_usage;

    

} ;

System_monitor *system_monitor_init(void);

#endif //_MODEL_H