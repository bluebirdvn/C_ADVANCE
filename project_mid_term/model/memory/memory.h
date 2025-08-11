
#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
typedef struct Memory_info Memory_info;
typedef struct Cpu_process_info Cpu_process_info;


Memory_info *Memory_info_init(void);


#endif // _MEMORY_H