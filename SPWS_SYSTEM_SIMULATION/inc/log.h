#ifndef LOG_H
#define LOG_H

#include <stdio.h> 
#include <stdarg.h> 

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel_t;

void log_message(LogLevel_t level, const char *format, ...);

#endif