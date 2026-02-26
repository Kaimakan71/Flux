/*
 * Vulkan RHI command buffer.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "VulkanCommandBuffer.hpp"

namespace Flux {

void VulkanCommandBuffer::free(void) {
    FLUX_LOG_DEBUG("Freeing command buffer...");
    this->pool->device->dispatch.vkFreeCommandBuffers(this->pool->device->device, this->pool->pool, 1, &this->buffer);
}

}
