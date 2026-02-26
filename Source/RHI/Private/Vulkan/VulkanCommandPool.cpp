/*
 * Vulkan RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "VulkanCommandPool.hpp"
#include "VulkanResult.hpp"

namespace Flux {

void VulkanCommandPool::destroy(void) {
    if (this->pool != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying command pool...");
        this->device->dispatch.vkDestroyCommandPool(this->device->device, this->pool, nullptr);
        this->pool = VK_NULL_HANDLE;
    }
}

Status VulkanCommandPool::create(VulkanDevice *device) {
    VkResult result;

    static VkCommandPoolCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = UINT32_MAX,
    };

    this->device = device;
    createInfo.queueFamilyIndex = this->device->deviceInfo.queueFamilies.graphicsIndex;

    FLUX_LOG_DEBUG("Creating command pool...");
    result = this->device->dispatch.vkCreateCommandPool(this->device->device, &createInfo, nullptr, &this->pool);
    if (result != VK_SUCCESS) {
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

}
