/*
 * Vulkan swapchain utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vkUtils.h"

#include <stdlib.h>

static VkSwapchainCreateInfoKHR swapchainCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = VK_NULL_HANDLE,
    .minImageCount = 0,
    .imageFormat = VK_FORMAT_MAX_ENUM,
    .imageColorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR,
    .imageExtent = {
        .width = 0,
        .height = 0
    },
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_MAX_ENUM,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = 0,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR,
    .clipped = VK_TRUE,
    .oldSwapchain = VK_NULL_HANDLE
};

static VkImageViewCreateInfo imageViewCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = VK_NULL_HANDLE,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = VK_FORMAT_MAX_ENUM,
    .components = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY
    },
    .subresourceRange = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    }
};

static VkFramebufferCreateInfo framebufferCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = 0,
    .attachmentCount = 0,
    .pAttachments = NULL,
    .width = UINT32_MAX,
    .height = UINT32_MAX,
    .layers = 1
};

static uint32_t
clampUInt32(uint32_t value, uint32_t minimum, uint32_t maximum)
{
    if (value < minimum) {
        return minimum;
    }

    if (value > maximum) {
        return maximum;
    }

    return value;
}

static VkSurfaceFormatKHR*
selectSurfaceFormat(uint32_t formatCount, VkSurfaceFormatKHR* pFormats)
{
    for (uint32_t f = 0; f < formatCount; f++) {
        if (pFormats[f].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return &pFormats[f];
        }
    }

    return &pFormats[0];
}

static VkPresentModeKHR
selectPresentMode(uint32_t modeCount, VkPresentModeKHR* pModes)
{
    /* Mailbox is preferred */
    for (uint32_t m = 0; m < modeCount; m++) {
        if (pModes[m] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    /* ... but FIFO is okay too */
    for (uint32_t m = 0; m < modeCount; m++) {
        if (pModes[m] == VK_PRESENT_MODE_FIFO_KHR) {
            return VK_PRESENT_MODE_FIFO_KHR;
        }
    }

    return pModes[0];
}

static VkExtent2D
selectImageExtent(GLFWwindow* window, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    int width, height;
    VkExtent2D extent;

    if (pSurfaceCapabilities->currentExtent.width != UINT32_MAX && pSurfaceCapabilities->currentExtent.height != UINT32_MAX) {
        return pSurfaceCapabilities->currentExtent;
    }

    /* Need size in *pixels* (to adjust for DPI) */
    glfwGetFramebufferSize(window, &width, &height);

    /* Use best possible valid size */
    extent.width = clampUInt32((uint32_t)width, pSurfaceCapabilities->minImageExtent.width, pSurfaceCapabilities->maxImageExtent.width);
    extent.height = clampUInt32((uint32_t)height, pSurfaceCapabilities->minImageExtent.height, pSurfaceCapabilities->maxImageExtent.height);

    return extent;
}

static uint32_t
selectImageCount(VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    uint32_t count;

    count = pSurfaceCapabilities->minImageCount + 1;
    if (pSurfaceCapabilities->maxImageCount != 0 && count > pSurfaceCapabilities->minImageCount) {
        count = pSurfaceCapabilities->maxImageCount;
    }

    return count;
}

void
vkUtilsDestroySwapchainFramebuffers(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkUtilsSwapchainInfo* pInfo)
{
    for (uint32_t i = 0; i < pInfo->imageCount; i++) {
        pDeviceDispatchTable->vkDestroyFramebuffer(device, pInfo->pFramebuffers[i], NULL);
    }

    free(pInfo->pFramebuffers);
    pInfo->pFramebuffers = NULL;
}

VkResult
vkUtilsCreateSwapchainFramebuffers(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkRenderPass renderPass, VkUtilsSwapchainInfo* pInfo)
{
    VkResult result;
    VkImageView attachments[1];

    pInfo->pFramebuffers = malloc(sizeof(*pInfo->pFramebuffers) * pInfo->imageCount);
    if (pInfo->pFramebuffers == NULL) {
        vkUtilsLogError(VK_ERROR_OUT_OF_HOST_MEMORY, "failed to allocate memory for framebuffer list");
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    framebufferCreateInfo.renderPass = renderPass;
    framebufferCreateInfo.attachmentCount = 1;
    framebufferCreateInfo.pAttachments = attachments;
    framebufferCreateInfo.width = pInfo->imageExtent.width;
    framebufferCreateInfo.height = pInfo->imageExtent.height;

    for (uint32_t i = 0; i < pInfo->imageCount; i++) {
        attachments[0] = pInfo->pImageViews[i];

        result = pDeviceDispatchTable->vkCreateFramebuffer(device, &framebufferCreateInfo, NULL, &pInfo->pFramebuffers[i]);
        if (result == VK_SUCCESS) {
            continue;
        }

        vkUtilsLogError(result, "failed to create framebuffer");

        /* Destroy previously created framebuffers */
        for (uint32_t f = 0; f < i; f++) {
            pDeviceDispatchTable->vkDestroyFramebuffer(device, pInfo->pFramebuffers[f], NULL);
        }

        free(pInfo->pFramebuffers);
        pInfo->pFramebuffers = NULL;
        return result;
    }

    return VK_SUCCESS;
}

void
vkUtilsDestroySwapchain(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkSwapchainKHR swapchain, VkUtilsSwapchainInfo* pInfo)
{
    if (pInfo->pImageViews != NULL) {
        for (uint32_t i = 0; i < pInfo->imageCount; i++) {
            pDeviceDispatchTable->vkDestroyImageView(device, pInfo->pImageViews[i], NULL);
        }

        free(pInfo->pImageViews);
        pInfo->pImageViews = NULL;
    }

    pDeviceDispatchTable->vkDestroySwapchainKHR(device, swapchain, NULL);
}

VkResult
vkUtilsCreateSwapchain(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkSurfaceKHR surface, GLFWwindow* window, VkUtilsGraphicsDeviceInfo* pDeviceInfo, VkSwapchainKHR* pSwapchain)
{
    VkResult result;
    uint32_t imageCount;
    VkExtent2D imageExtent;
    VkSurfaceFormatKHR* pSurfaceFormat;
    VkPresentModeKHR presentMode;
    VkSwapchainKHR swapchain;
    VkImage* pImages;

    /* So vkUtilsDestroySwapchain() can be used safely */
    pDeviceInfo->swapchain.pImageViews = NULL;

    /* TODO: Allow the caller to provide preferred present modes */
    imageCount = selectImageCount(&pDeviceInfo->surface.capabilities);
    imageExtent = selectImageExtent(window, &pDeviceInfo->surface.capabilities);
    pSurfaceFormat = selectSurfaceFormat(pDeviceInfo->surface.formatCount, pDeviceInfo->surface.pFormats);
    presentMode = selectPresentMode(pDeviceInfo->surface.presentModeCount, pDeviceInfo->surface.pPresentModes);

    vkUtilsLogDebug("image count: %u", imageCount);
    vkUtilsLogDebug("image extent: %ux%u", imageExtent.width, imageExtent.height);
    vkUtilsLogDebug("surface format: format=%d, colorSpace=%d", pSurfaceFormat->format, pSurfaceFormat->colorSpace);
    vkUtilsLogDebug("present mode: %s", vkUtilsGetPresentModeString(presentMode));

    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = pSurfaceFormat->format;
    swapchainCreateInfo.imageColorSpace = pSurfaceFormat->colorSpace;
    swapchainCreateInfo.imageExtent = imageExtent;
    swapchainCreateInfo.preTransform = pDeviceInfo->surface.capabilities.currentTransform;
    swapchainCreateInfo.presentMode = presentMode;

    /* Share queue families if needed */
    if (pDeviceInfo->queueFamilies.count > 1) {
        vkUtilsLogDebug("image sharing mode: concurrent");
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = pDeviceInfo->queueFamilies.count;
        swapchainCreateInfo.pQueueFamilyIndices = pDeviceInfo->queueFamilies.indices;
    } else {
        vkUtilsLogDebug("image sharing mode: exclusive");
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = NULL;
    }

    result = pDeviceDispatchTable->vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, &swapchain);
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to create swapchain");
        return result;
    }

    result = vkUtilsGetSwapchainImages(pDeviceDispatchTable->vkGetSwapchainImagesKHR, device, swapchain, &imageCount, &pImages);
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to get images");
        vkUtilsDestroySwapchain(device, pDeviceDispatchTable, swapchain, &pDeviceInfo->swapchain);
        return result;
    }
    vkUtilsLogDebug("final image count: %u", imageCount);

    pDeviceInfo->swapchain.pImageViews = malloc(sizeof(*pDeviceInfo->swapchain.pImageViews) * imageCount);
    if (pDeviceInfo->swapchain.pImageViews == NULL) {
        vkUtilsLogError(VK_ERROR_OUT_OF_HOST_MEMORY, "failed to allocate memory for image views");
        free(pImages);
        vkUtilsDestroySwapchain(device, pDeviceDispatchTable, swapchain, &pDeviceInfo->swapchain);
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    imageViewCreateInfo.format = swapchainCreateInfo.imageFormat;
    for (uint32_t i = 0; i < imageCount; i++) {
        imageViewCreateInfo.image = pImages[i];

        result = pDeviceDispatchTable->vkCreateImageView(device, &imageViewCreateInfo, NULL, &pDeviceInfo->swapchain.pImageViews[i]);
        if (result == VK_SUCCESS) {
            continue;
        }

        vkUtilsLogError(result, "failed to create image view");

        /* Destroy previously created views */
        for (uint32_t v = 0; v < i; v++) {
            pDeviceDispatchTable->vkDestroyImageView(device, pDeviceInfo->swapchain.pImageViews[v], NULL);
        }

        free(pImages);
        vkUtilsDestroySwapchain(device, pDeviceDispatchTable, swapchain, &pDeviceInfo->swapchain);
        return result;
    }

    pDeviceInfo->swapchain.imageCount = imageCount;
    pDeviceInfo->swapchain.imageFormat = swapchainCreateInfo.imageFormat;
    pDeviceInfo->swapchain.imageExtent = swapchainCreateInfo.imageExtent;

    free(pImages);
    *pSwapchain = swapchain;
    return VK_SUCCESS;
}
