/*
 * OpenGL logging utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/glUtils.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

void
__glUtilsLogDebug(const char *functionName, const char *format, ...)
{
    va_list argumentList;

    printf("\033[1;90m[glUtils debug]\033[0m %s(): ", functionName);
    va_start(argumentList, format);
    vprintf(format, argumentList);
    va_end(argumentList);
    putchar('\n');
}

void
__glUtilsLogError(const char *functionName, GLenum error, const char *format, ...)
{
    va_list argumentList;

    fprintf(stderr, "\033[1;91m[OpenGL error]\033[0m %s(): ", functionName);
    va_start(argumentList, format);
    vfprintf(stderr, format, argumentList);
    va_end(argumentList);
    fprintf(stderr, ": %s (0x%08x): %s\n", glUtilsGetErrorName(error), (uint32_t)error, glUtilsGetErrorDescription(error));
}
