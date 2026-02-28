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

    VulkanRHI *rhi;

    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    uint32_t swapchainImageCount;
    VkSurfaceFormatKHR *swapchainImageFormat;
    VkPresentModeKHR swapchainPresentMode;
    VkImageView *swapchainImageViews;

    VkResult selectPhysicalDevice(void);

    void selectSwapchainParameters(Window window);

    VkResult getSwapchainImages(uint32_t *imageCount, VkImage **images);

    VkResult createSurface(Window window);

    VkResult createDevice(void);

    VkResult createSwapchain(Window window);

    VkResult createRenderPass(void);

    VkResult createSwapchainFramebuffers(void);

    VkResult createAll(Window window);

public:

    VulkanDeviceInfo deviceInfo;
    VkDevice device;
    VulkanDeviceDispatch dispatch;
    VkRenderPass renderPass;
    VkExtent2D swapchainImageExtent;
    VkSwapchainKHR swapchain;
    VkFramebuffer *swapchainFramebuffers;

    static const char *getPhysicalDeviceTypeString(VkPhysicalDeviceType type);

    static const char *getPresentModeString(VkPresentModeKHR mode);

    VkBool32 deviceIsCompatible(VkPhysicalDevice device, VulkanDeviceInfo *info);

    Status create(VulkanRHI *rhi, Window window);

    virtual Status createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipeline);

    virtual Status createCommandPool(RHICommandPool **pool);

    virtual Status createRenderingAgent(RHIRenderingAgent **agent);

    virtual void destroy(void);

};

}
