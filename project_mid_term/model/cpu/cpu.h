#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#define NUM_FIELDS 8

typedef struct CPU_times CPU_times;
typedef struct CPU_manager CPU_manager;

CPU_manager *CPU_manager_init(void);


#endif // _CPU_H
