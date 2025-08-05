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
    VkUtilsInstanceDispatchTable dispatchTable;

    VkInstance instance;
} RhiVulkanData;

typedef struct FluxRhiDevice_T {
    VkUtilsGraphicsDeviceInfo info;

    VkSurfaceKHR surface;
    VkDevice device;
    VkSwapchainKHR swapchain;
    VkRenderPass renderPass;
} RhiVulkanDevice;

FluxStatus translateVkResult(VkResult result);

FluxStatus rhiVulkanCreateDevice(FluxRhi *rhi, GLFWwindow *window, RhiVulkanDevice **deviceOut);
void rhiVulkanDestroyDevice(FluxRhi *rhi, RhiVulkanDevice *device);

#endif /* !_RHI_VULKAN_H */
