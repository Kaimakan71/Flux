/*
 * Vulkan RHI command buffer.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanCommandPool.hpp"

namespace Flux {

class VulkanCommandBuffer: public RHICommandBuffer {
public:

    VulkanCommandPool *pool;

    VkCommandBuffer buffer;

    virtual void free(void);

};

}
