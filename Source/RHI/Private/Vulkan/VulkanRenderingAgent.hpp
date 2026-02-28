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

    static const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    VkResult createSyncObjects(void);

    void drawFrame(uint32_t currentFrame, VkPipeline pipeline);

public:

    VulkanDevice &device;

    VulkanCommandPool *commandPool;
    VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore imageAvailable[MAX_FRAMES_IN_FLIGHT];
    VkFence inFlight[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderComplete[MAX_FRAMES_IN_FLIGHT];

    VulkanRenderingAgent(VulkanDevice &device);

    Status create(void);

    virtual void present(RHIPipeline &pipeline);

    virtual void destroy(void);

};

}
