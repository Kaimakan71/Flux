/*
 * Vulkan RHI device.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanRHI.hpp"

namespace Flux {

class VulkanDevice: public RHIDevice {
private:

    VkResult selectPhysicalDevice(void);

    void selectSwapchainParameters(void);

    VkResult getSwapchainImages(uint32_t *imageCount, VkImage **images);

    VkResult createSurface(void);

    VkResult createDevice(void);

    VkResult createSwapchain(void);

    VkResult createRenderPass(void);

    VkResult createSwapchainFramebuffers(void);

    VkResult createAll(void);

public:

    VulkanRHI &rhi;
    Window &window;

    VkSurfaceKHR surface;
    VulkanDeviceInfo info;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VulkanDeviceDispatch dispatch;
    uint32_t swapchainImageCount;
    VkSurfaceFormatKHR *swapchainImageFormat;
    VkExtent2D swapchainImageExtent;
    VkPresentModeKHR swapchainPresentMode;
    VkSwapchainKHR swapchain;
    VkRenderPass renderPass;
    VkImageView *swapchainImageViews;
    VkFramebuffer *swapchainFramebuffers;

    static const char *getPhysicalDeviceTypeString(VkPhysicalDeviceType type);

    static const char *getPresentModeString(VkPresentModeKHR mode);

    VulkanDevice(VulkanRHI &rhi, Window &window);

    VkBool32 deviceIsCompatible(VkPhysicalDevice device, VulkanDeviceInfo *info);

    Status create(void);

    virtual Status createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipeline);

    virtual Status createCommandPool(RHICommandPool **pool);

    virtual Status createRenderingAgent(RHIRenderingAgent **agent);

    virtual void destroy(void);

};

}
