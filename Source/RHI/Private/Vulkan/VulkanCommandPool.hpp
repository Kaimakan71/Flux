/*
 * Vulkan RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanDevice.hpp"

namespace Flux {

class VulkanCommandPool: public RHICommandPool {
public:

    VulkanDevice &device;

    VkCommandPool pool;

    VulkanCommandPool(VulkanDevice &device);

    Status create(void);

    virtual Status allocateCommandBuffers(uint32_t bufferCount, RHICommandBuffer **buffers);

    virtual void freeCommandBuffers(uint32_t bufferCount, RHICommandBuffer *buffers);

    virtual void destroy(void);

};

}
