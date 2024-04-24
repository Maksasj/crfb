#ifndef _CRFB_LOGGER_H_
#define _CRFB_LOGGER_H_

#include <stdio.h>
#include <stdarg.h>

typedef enum CRFB_LOG_LEVEL {
    CRFB_INFO,
    CRFB_WARNING,
    CRFB_ERROR
} CRFB_LOG_LEVEL;

char* crfb_stringify_log_level(CRFB_LOG_LEVEL logLevel);
void crfb_log(CRFB_LOG_LEVEL logLevel, const char *format, ...);

#define CRFB_LOG(logLevel, ...) crfb_log(logLevel, __VA_ARGS__)

#endif