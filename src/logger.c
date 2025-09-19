#include <arpa/inet.h>
#include <netinet/in.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "logger.h"
#include <stdarg.h>
#include <time.h>

const char *level_names[]  = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
const char *level_colors[] = {COLOR_CYAN,   COLOR_MAGENTA, COLOR_GREEN,
                              COLOR_YELLOW, COLOR_RED,     COLOR_RED};

logger lg;

int init_logger(logger *lg, log_level level, int use_colors, char *time_format, FILE *output) {
    if (!lg) {
        fprintf(stderr, "Logger pointer is NULL\n");
        return 0;
    }

    lg->level       = level;
    lg->use_colors  = use_colors;
    lg->output      = output ? output : stdout;
    lg->time_format = time_format ? time_format : DEFAULT_TIME_FORMAT;

    return 1;
}

void log_message(log_level level, const char *fmt, ...) {
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);
    char       timebuf[20];
    strftime(timebuf, sizeof(timebuf), lg.time_format, t);

    const char *color = lg.use_colors ? level_colors[level] : "";
    const char *reset = lg.use_colors ? COLOR_RESET : "";

    fprintf(lg.output, "%s [%s%s%s] ", timebuf, color, level_names[level], reset);

    va_list args;
    va_start(args, fmt);
    vfprintf(lg.output, fmt, args);
    va_end(args);

    fprintf(lg.output, "\n");
    fflush(lg.output);
}

void log_trace(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message(LOG_TRACE, fmt, args);
    va_end(args);
}

void log_debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message(LOG_DEBUG, fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message(LOG_INFO, fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message(LOG_ERROR, fmt, args);
    va_end(args);
}

void log_warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message(LOG_WARN, fmt, args);
    va_end(args);
}

void log_fatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message(LOG_FATAL, fmt, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
