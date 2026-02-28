/*
 * Vulkan RHI rendering agent.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanDevice.hpp"

namespace Flux {

class VulkanRenderingAgent: public RHIRenderingAgent {
private:

    VulkanDevice *device;

    VkSemaphore imageAvailable;
    VkFence inFlight;
    VkSemaphore renderComplete;

    VkResult createSyncObjects(void);

    void drawFrame(VkCommandBuffer commandBuffer, VkPipeline pipeline);

public:

    Status create(VulkanDevice *device);

    virtual void present(Window window, RHICommandBuffer *commandBuffer, RHIPipeline *pipeline);

    virtual void destroy(void);

};

}
