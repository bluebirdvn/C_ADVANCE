#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

static FILE* log_file = NULL;
static log_level_t init_level = LOG_INFO;

static const char* log_level_strings[] = {
	    "EMERGENCY", "ALERT", "CRITICAL", "ERROR", "WARNING", "NOTICE", "INFO", "DEBUG"
};

int logger_init(const char* filename, log_level_t initial_level) {
	if (filename != NULL) {
		log_file = fopen(filename, "a");
		if (log_file == NULL) {
			perror("Open file failed");
			return 1;
		}
	}
	log_set_level(initial_level);
	return 0;
}


void log_set_level(log_level_t level) {
	init_level = level;
}

void logger_clean(void) {
	if (log_file) {
		fclose(log_file);
	}
	log_file = NULL;
}


void _log_message(log_level_t level, const char* file, int line, const char* fmt, ...) {
	if (level > init_level) {
		return;
	}

	char timestamp[20];
	time_t cur_time = time(NULL);
	struct tm *tm_info = localtime(&cur_time);
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

	char header[256];
	snprintf(header, sizeof(header), "[%s] [%s] [%s:%d]", timestamp, log_level_strings[level], file, line);
	
	va_list args;
	
	va_start(args, fmt);
	FILE* console_stream = (level <= LOG_ERROR) ? stderr : stdout;
	fprintf(console_stream, "%s", header);
	vfprintf(console_stream, fmt, args);
	fprintf(console_stream, "\n");
	fflush(console_stream); // Đẩy ra ngay lập tức
	va_end(args);

	if (log_file) {
		va_list args_copy;
		va_copy(args_copy, args);
		va_start(args_copy, fmt);
		fprintf(log_file, "%s", header);
	        vfprintf(log_file, fmt, args_copy);
	        fprintf(log_file, "\n");
	        fflush(log_file); 
	        va_end(args_copy);
	}
}


















