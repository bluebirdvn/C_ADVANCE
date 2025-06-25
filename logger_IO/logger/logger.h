#ifndef _LOGGER_H
#define _LOGGER_H

typedef enum {
	LOG_EMERGENCY = 0, // can't use system
        LOG_ALERT     = 1, // need action imediatively
	LOG_CRITICAL  = 2, // 
	LOG_ERROR     = 3, // error
	LOG_WARNING   = 4, // alert
	LOG_NOTICE    = 5, // importance information
	LOG_INFO      = 6, // information
	LOG_DEBUG     = 7  // Thông tin gỡ lỗi
} log_level_t;

int logger_init(const char* filename, log_level_t initial_level);

void log_set_level(log_level_t level);

void logger_clean(void);

void _log_message(log_level_t level, const char* file, int line, const char* fmt, ...);

#define log_message(level, ...) _log_message(level, __FILE__, __LINE__, __VA_ARGS__)
#endif //Logger.h
