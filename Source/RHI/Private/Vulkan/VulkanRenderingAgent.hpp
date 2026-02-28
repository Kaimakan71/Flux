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

    VkResult createSyncObjects(void);

    void drawFrame(VkCommandBuffer commandBuffer, VkPipeline pipeline);

public:

    VulkanDevice &device;

    VkSemaphore imageAvailable;
    VkFence inFlight;
    VkSemaphore renderComplete;

    VulkanRenderingAgent(VulkanDevice &device);

    Status create(void);

    virtual void present(RHICommandBuffer &commandBuffer, RHIPipeline &pipeline);

    virtual void destroy(void);

};

}
