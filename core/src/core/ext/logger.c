#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"

static const char *logger_level_names[] = {"DEBUG", "INFO", "WARN", "FATAL"};

static void (*cleanup_cb)(void);

void
logger_set_cleanup_callback(void (*cb)(void))
{
	cleanup_cb = cb;
}

void
logger_write(log_level level, const char *file, s32 line, const char *fmt, ...)
{
	va_list args;
	fprintf(stderr, "%-5s %s:%d: ", logger_level_names[level], file, line);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	fflush(stderr);

	if (level == LOG_FATAL)
	{
		if (cleanup_cb) cleanup_cb();	
		exit(1);
	}
}
