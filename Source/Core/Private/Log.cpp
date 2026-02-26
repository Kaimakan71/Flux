/*
 * Logging utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cstdarg>
#include <cstdint>
#include "Log.hpp"

namespace Flux::Log {

void info(const char *file, unsigned int line, const char *function, const char *format, ...) {
    va_list args;

    printf("\e[1;94m[info]\e[0m %s:%u: %s(): ", file, line, function);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    putchar('\n');
}

void warning(const char *file, unsigned int line, const char *function, const char *format, ...) {
    va_list args;

    printf("\e[1;93m[warning]\e[0m %s:%u: %s(): ", file, line, function);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    putchar('\n');
}

void error(const char *file, unsigned int line, const char *function, const char *format, ...) {
    va_list args;

    fprintf(stderr, "\e[1;91m[error]\e[0m %s:%u: %s(): ", file, line, function);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputc('\n', stderr);
}

void status(const char *file, unsigned int line, const char *function, Status status, const char *format, ...) {
    va_list args;
    const char *prefix;
    FILE *stream;

    if (status == Status::success) {
        prefix = "\e[1;92m[success]\e[0m %s:%u: %s(): ";
        stream = stdout;
    } else {
        prefix = "\e[1;91m[error]\e[0m %s:%u: %s(): ";
        stream = stderr;
    }

    fprintf(stream, prefix, file, line, function);
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    fprintf(stream, ": %s (0x%08x): %s\n", getStatusName(status), (uint32_t) status, getStatusDescription(status));
}

}
