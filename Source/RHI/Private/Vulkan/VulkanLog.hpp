/*
 * VkResult logging utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <string>
#include "Status.hpp"

#define FLUX_LOG_VULKAN_WARNING(result, ...) ::Flux::VulkanLog::warning(::Flux::VulkanLog::_shortenPath(__builtin_FILE()), __builtin_LINE(), __func__, (result), __VA_ARGS__)
#define FLUX_LOG_VULKAN_ERROR(result, ...) ::Flux::VulkanLog::error(::Flux::VulkanLog::_shortenPath(__builtin_FILE()), __builtin_LINE(), __func__, (result), __VA_ARGS__)

namespace Flux::VulkanLog {

static constexpr const char *const _shortenPath(const char *path) {
    std::string_view view = std::string_view(path);
    return view.substr(view.find("Source") + std::string::traits_type::length("Source") + 1).data();
}

void warning(const char *file, unsigned int line, const char *function, VkResult result, const char *format, ...);
void error(const char *file, unsigned int line, const char *function, VkResult result, const char *format, ...);

}
