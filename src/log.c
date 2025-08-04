/*
 * Logging utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <flux/log.h>
#include <stdarg.h>
#include <stdio.h>

void
__fluxLogDebug(const char *function, const char *format, ...)
{
    va_list arguments;

    printf("\033[1;90m[debug]\033[0m %s(): ", function);
    va_start(arguments, format);
    vprintf(format, arguments);
    va_end(arguments);
    putchar('\n');
}
