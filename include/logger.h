#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define COLOR_RESET  "\033[0m"
#define COLOR_GREEN  "\033[32m" // success/info
#define COLOR_YELLOW "\033[33m" // warning
#define COLOR_RED    "\033[31m" // error/fatal
#define COLOR_CYAN   "\033[36m" // trace

#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"

#define DEFAULT_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

typedef enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL } log_level;

typedef struct {
    log_level level;
    int       use_colors;
    char     *time_format;
    FILE     *output; // (stdout, stderr, file)
} logger;

extern logger lg;

int  init_logger(logger *lg, log_level level, int use_colors, char *time_format, FILE *output);
void log_message(log_level level, const char *fmt, ...);
void log_trace(const char *fmt, ...);
void log_debug(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_fatal(const char *fmt, ...);
#endif // LOGGER_H