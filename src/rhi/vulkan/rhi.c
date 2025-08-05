/*
 * Vulkan RHI.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vulkan.h"

#include <flux/log.h>
#include <stdbool.h>
#include <stdlib.h>

static VkBool32 libraryLoaded = VK_FALSE;

static VkResult
loadLibrary(void)
{
    VkResult result;

    if (libraryLoaded) {
        return VK_SUCCESS;
    }

    fluxLogDebug("checking for Vulkan support...");
    if (glfwVulkanSupported() != GLFW_TRUE) {
        return VK_ERROR_INCOMPATIBLE_DRIVER;
    }

    fluxLogDebug("loading global symbols...");
    result = vkUtilsLoadGlobalSymbols((VkUtilsGetSymbolAddress)glfwGetInstanceProcAddress);
    if (result != VK_SUCCESS) {
        return result;
    }

    libraryLoaded = VK_TRUE;
    return VK_SUCCESS;
}

static void
rhiVulkanShutdown(FluxRhi *rhi)
{
    RhiVulkanData *data;

    if (rhi == NULL || rhi->data == NULL) {
        return;
    }

    fluxLogDebug("shutting down RHI...");
    data = rhi->data;

    if (data->instance != VK_NULL_HANDLE) {
        fluxLogDebug("destroying instance...");
        data->dispatchTable.vkDestroyInstance(data->instance, NULL);
    }

    free(data);
    rhi->data = NULL;
}

FluxStatus
translateVkResult(VkResult result)
{
    switch (result) {
    case VK_SUCCESS:
        return FLUX_STATUS_SUCCESS;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return FLUX_STATUS_OUT_OF_HOST_MEMORY;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return FLUX_STATUS_OUT_OF_DEVICE_MEMORY;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return FLUX_STATUS_INCOMPATIBLE_DRIVER;
    case VK_ERROR_UNKNOWN:
    default:
        return FLUX_STATUS_UNKNOWN_ERROR;
    }
}

FluxStatus
rhiVulkanInitialize(FluxRhi *rhi)
{
    RhiVulkanData *data;
    VkResult result;

    if (rhi == NULL) {
        return FLUX_STATUS_INVALID_ARGUMENT;
    }

    fluxLogDebug("initializing RHI...");

    result = loadLibrary();
    if (result != VK_SUCCESS) {
        return translateVkResult(result);
    }

    data = malloc(sizeof(*data));
    if (data == NULL) {
        return FLUX_STATUS_OUT_OF_HOST_MEMORY;
    }

    fluxLogDebug("creating instance...");
    result = vkUtilsCreateInstance("Flux application", &data->instance, &data->dispatchTable);
    if (result != VK_SUCCESS) {
        free(data);
        return translateVkResult(result);
    }

    rhi->data = data;
    rhi->createDevice = rhiVulkanCreateDevice;
    rhi->destroyDevice = rhiVulkanDestroyDevice;
    rhi->shutdown = rhiVulkanShutdown;

    return FLUX_STATUS_SUCCESS;
}
