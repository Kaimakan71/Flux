/*
 * Vulkan device.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vulkan.h"

#include <flux/log.h>
#include <stdlib.h>
#include <string.h>

static VkAttachmentDescription colorAttachmentDescription = {
    .flags = 0,
    .format = VK_FORMAT_MAX_ENUM,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
};

static const VkAttachmentReference colorAttachmentReference = {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
};

static const VkSubpassDescription subpassDescription = {
    .flags = 0,
    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
    .inputAttachmentCount = 0,
    .pInputAttachments = NULL,
    .colorAttachmentCount = 1,
    .pColorAttachments = &colorAttachmentReference,
    .pResolveAttachments = NULL,
    .pDepthStencilAttachment = NULL,
    .preserveAttachmentCount = 0,
    .pPreserveAttachments = NULL
};

static const VkSubpassDependency subpassDependency = {
    .srcSubpass = VK_SUBPASS_EXTERNAL,
    .dstSubpass = 0,
    .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    .dependencyFlags = 0
};

static const VkRenderPassCreateInfo renderPassCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = &colorAttachmentDescription,
    .subpassCount = 1,
    .pSubpasses = &subpassDescription,
    .dependencyCount = 1,
    .pDependencies = &subpassDependency
};

static VkResult
rhiVulkanInitializeDevice(RhiVulkanData *rhiData, GLFWwindow *window, RhiVulkanDevice *device)
{
    VkResult result;

    fluxLogDebug("creating surface...");
    result = glfwCreateWindowSurface(rhiData->instance, window, NULL, &device->surface);
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to create surface");
        return result;
    }

    fluxLogDebug("creating graphics device...");
    result = vkUtilsCreateGraphicsDevice(rhiData->instance, &rhiData->dispatchTable, device->surface, &device->info, &device->device);
    if (result != VK_SUCCESS) {
        return result;
    }

    fluxLogDebug("creating swapchain...");
    result = vkUtilsCreateSwapchain(device->device, &device->info.dispatchTable, device->surface, window, &device->info, &device->swapchain);
    if (result != VK_SUCCESS) {
        return result;
    }

    colorAttachmentDescription.format = device->info.swapchain.imageFormat;
    fluxLogDebug("creating render pass...");
    result = device->info.dispatchTable.vkCreateRenderPass(device->device, &renderPassCreateInfo, NULL, &device->renderPass);
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to create render pass");
        return result;
    }

    fluxLogDebug("creating swapchain framebuffers...");
    result = vkUtilsCreateSwapchainFramebuffers(device->device, &device->info.dispatchTable, device->renderPass, &device->info.swapchain);
    if (result != VK_SUCCESS) {
        return result;
    }

    return VK_SUCCESS;
}

void
rhiVulkanDestroyDevice(FluxRhi *rhi, RhiVulkanDevice *device)
{
    RhiVulkanData *rhiData;

    if (rhi == NULL || rhi->data == NULL || device == NULL) {
        return;
    }

    fluxLogDebug("destroying device...");
    rhiData = rhi->data;

    if (device->info.swapchain.pFramebuffers != NULL) {
        fluxLogDebug("destroying swapchain framebuffers...");
        vkUtilsDestroySwapchainFramebuffers(device->device, &device->info.dispatchTable, &device->info.swapchain);
    }

    if (device->renderPass != VK_NULL_HANDLE) {
        fluxLogDebug("destroying render pass...");
        device->info.dispatchTable.vkDestroyRenderPass(device->device, device->renderPass, NULL);
    }

    if (device->swapchain != VK_NULL_HANDLE) {
        fluxLogDebug("destroying swapchain...");
        vkUtilsDestroySwapchain(device->device, &device->info.dispatchTable, device->swapchain, &device->info.swapchain);
    }

    if (device->device != VK_NULL_HANDLE) {
        fluxLogDebug("destroying graphics device...");
        vkUtilsDestroyGraphicsDevice(device->device, &device->info);
    }

    if (device->surface != VK_NULL_HANDLE) {
        fluxLogDebug("destroying surface...");
        rhiData->dispatchTable.vkDestroySurfaceKHR(rhiData->instance, device->surface, NULL);
    }

    free(device);
}

FluxStatus
rhiVulkanCreateDevice(FluxRhi *rhi, GLFWwindow *window, RhiVulkanDevice **deviceOut)
{
    RhiVulkanDevice *device;
    VkResult result;

    if (rhi == NULL || window == NULL || deviceOut == NULL) {
        return FLUX_STATUS_INVALID_ARGUMENT;
    }

    fluxLogDebug("creating device...");

    device = malloc(sizeof(*device));
    if (device == NULL) {
        return FLUX_STATUS_OUT_OF_HOST_MEMORY;
    }

    memzero(device, sizeof(*device));
    result = rhiVulkanInitializeDevice(rhi->data, window, device);
    if (result != VK_SUCCESS) {
        rhiVulkanDestroyDevice(rhi, device);
        return translateVkResult(result);
    }

    *deviceOut = device;
    return FLUX_STATUS_SUCCESS;
}
