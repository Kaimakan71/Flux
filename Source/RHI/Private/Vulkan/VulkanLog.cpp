/*
 * VkResult logging utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cstdarg>
#include <cstdint>
#include "VulkanLog.hpp"
#include "VulkanResult.hpp"

namespace Flux::VulkanLog {

void warning(const char *file, unsigned int line, const char *function, VkResult result, const char *format, ...) {
    va_list args;

    printf("\e[1;93m[Vulkan warning]\e[0m %s:%u: %s(): ", file, line, function);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(": %s (0x%08x): %s\n", VulkanResult::getName(result), (uint32_t) result, VulkanResult::getDescription(result));
}

void error(const char *file, unsigned int line, const char *function, VkResult result, const char *format, ...) {
    va_list args;

    fprintf(stderr, "\e[1;91m[Vulkan error]\e[0m %s:%u: %s(): ", file, line, function);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, ": %s (0x%08x): %s\n", VulkanResult::getName(result), (uint32_t) result, VulkanResult::getDescription(result));
}

}
