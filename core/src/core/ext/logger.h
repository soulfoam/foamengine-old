#pragma once

#include "sugar.h"

typedef enum
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_FATAL
} log_level;

#define log_debug(...) logger_write(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) logger_write(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  logger_write(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) logger_write(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void logger_write(log_level level, const char *file, s32 line, const char *fmt, ...);

void logger_set_cleanup_callback(void (*cb)(void));
