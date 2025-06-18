#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>   

#define CURRENT_LOG_LEVEL LOG_LEVEL_INFO 

void log_message(LogLevel_t level, const char *format, ...) {
    if (level < CURRENT_LOG_LEVEL) {
        return; 
    }

    time_t rawtime;
    struct tm *info;
    char timestamp_buffer[80]; 

    time(&rawtime);             
    info = localtime(&rawtime); 

    strftime(timestamp_buffer, sizeof(timestamp_buffer), "%Y-%m-%d %H:%M:%S", info);

   
    printf("[%s] ", timestamp_buffer);
    switch (level) {
        case LOG_LEVEL_DEBUG: printf("[DEBUG] "); break;
        case LOG_LEVEL_INFO:  printf("[INFO]  "); break;
        case LOG_LEVEL_WARN:  printf("[WARN]  "); break;
        case LOG_LEVEL_ERROR: printf("[ERROR] "); break;
        default: printf("[UNKNOWN] "); break; 
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args); 
    va_end(args);
    printf("\n"); 
}