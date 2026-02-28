/*
 * Vulkan RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanLog.hpp"
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
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create command pool");
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

Status VulkanCommandPool::allocateCommandBuffers(uint32_t bufferCount, RHICommandBuffer **buffersOut) {
    VkResult result;
    VkCommandBuffer *vkBuffers;
    VulkanCommandBuffer *buffers;

    static VkCommandBufferAllocateInfo allocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = this->pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = bufferCount,
    };

    if (bufferCount < 1) {
        FLUX_LOG_ERROR("Invalid buffer count %u", bufferCount);
        return Status::invalidArgument;
    }

    vkBuffers = new(std::nothrow) VkCommandBuffer[bufferCount];
    if (vkBuffers == nullptr) {
        return Status::hostAllocationFailed;
    }

    buffers = new(std::nothrow) VulkanCommandBuffer[bufferCount];
    if (buffers == nullptr) {
        delete[] vkBuffers;
        return Status::hostAllocationFailed;
    }

    FLUX_LOG_DEBUG("Allocating command buffers...");
    result = this->device->dispatch.vkAllocateCommandBuffers(this->device->device, &allocateInfo, vkBuffers);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to allocate %u command buffer(s)", bufferCount);
        delete[] buffers;
        delete[] vkBuffers;
        return VulkanResult::getStatus(result);
    }

    for (uint32_t b = 0; b < bufferCount; b++) {
        buffers[b].pool = this;
        buffers[b].buffer = vkBuffers[b];
    }

    delete[] vkBuffers;
    *buffersOut = buffers;
    return Status::success;
}

void VulkanCommandPool::freeCommandBuffers(uint32_t bufferCount, RHICommandBuffer *buffers) {
    VkCommandBuffer *vkBuffers;

    if (bufferCount < 1) {
        return;
    }

    vkBuffers = new(std::nothrow) VkCommandBuffer[bufferCount];
    if (vkBuffers == nullptr) {
        return;
    }

    for (uint32_t b = 0; b < bufferCount; b++) {
        vkBuffers[b] = (reinterpret_cast<VulkanCommandBuffer *>(buffers))[b].buffer;
    }

    FLUX_LOG_DEBUG("Freeing command buffers...");
    this->device->dispatch.vkFreeCommandBuffers(this->device->device, this->pool, bufferCount, vkBuffers);

    delete[] vkBuffers;
}

}
