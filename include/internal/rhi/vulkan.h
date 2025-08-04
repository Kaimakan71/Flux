/*
 * Vulkan RHI.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _RHI_VULKAN_H
#define _RHI_VULKAN_H

#include "internal/rhi/vkUtils.h"

#include <flux/rhi.h>

typedef struct {
    VkInstance instance;
    VkUtilsInstanceDispatchTable dispatchTable;
} RhiVulkanData;

FluxStatus translateVkResult(VkResult result);

#endif /* !_RHI_VULKAN_H */
