/*
 * Vulkan logging utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vkUtils.h"

#include <stdarg.h>
#include <stdio.h>

void
__vkUtilsLogDebug(const char* pFunctionName, const char* pFormat, ...)
{
    va_list argumentList;

    printf("\033[1;90m[vkUtils debug]\033[0m %s(): ", pFunctionName);
    va_start(argumentList, pFormat);
    vprintf(pFormat, argumentList);
    va_end(argumentList);
    putchar('\n');
}

void
__vkUtilsLogError(const char* pFunctionName, VkResult result, const char* pFormat, ...)
{
    va_list argumentList;

    fprintf(stderr, "\033[1;91m[Vulkan error]\033[0m %s(): ", pFunctionName);
    va_start(argumentList, pFormat);
    vfprintf(stderr, pFormat, argumentList);
    va_end(argumentList);
    fprintf(stderr, ": %s (0x%08x): %s\n", vkUtilsGetResultName(result), (uint32_t)result, vkUtilsGetResultDescription(result));
}
