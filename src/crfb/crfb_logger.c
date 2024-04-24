#include "crfb_logger.h"

char* crfb_stringify_log_level(CRFB_LOG_LEVEL logLevel) {
    switch (logLevel) {
        case CRFB_INFO:       { return "INFO"; }
        case CRFB_WARNING:    { return "WARNING"; }
        case CRFB_ERROR:      { return "ERROR"; }
    }

    return NULL;
}

void crfb_log(CRFB_LOG_LEVEL logLevel, const char *format, ...) {
    va_list args;
    va_start(args, format);

    printf("[%s] ", crfb_stringify_log_level(logLevel));
    vprintf(format, args);
    printf("\n");

    va_end(args);
}
