/*
 * Vulkan graphics device utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vkUtils.h"

#include <stdlib.h>
#include <string.h>

#define LOAD_DEVICE_PROC(name) pDispatchTable->name = (PFN_##name)pInstanceDispatchTable->vkGetDeviceProcAddr(device, #name);

static const char* requiredExtensionNames[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static VkDeviceCreateInfo deviceInfo = {
    /* NOTE: enabledLayerCount and ppEnabledLayerNames are deprecated */
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueCreateInfoCount = 0,
    .pQueueCreateInfos = NULL,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .enabledExtensionCount = ARRAY_LENGTH(requiredExtensionNames),
    .ppEnabledExtensionNames = requiredExtensionNames,
    .pEnabledFeatures = NULL
};

static VkResult
getQueueFamiliesInfo(VkInstance instance, VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkPhysicalDevice device, VkUtilsQueueFamiliesInfo* pInfo)
{
    VkResult result;
    uint32_t propertyCount, graphicsIndex, presentIndex;
    VkQueueFamilyProperties *pProperties;

    result = vkUtilsGetPhysicalDeviceQueueFamilyProperties(
        pInstanceDispatchTable->vkGetPhysicalDeviceQueueFamilyProperties,
        device,
        &propertyCount,
        &pProperties
    );
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to get queue family properties");
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    graphicsIndex = UINT32_MAX;
    presentIndex = UINT32_MAX;
    for (uint32_t p = 0; p < propertyCount; p++) {
        if (pProperties[p].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsIndex = p;
        }

        if (glfwGetPhysicalDevicePresentationSupport(instance, device, p) == GLFW_TRUE) {
            presentIndex = p;
        }

        if (graphicsIndex != UINT32_MAX && presentIndex != UINT32_MAX) {
            break;
        }
    }

    free(pProperties);

    if (graphicsIndex == UINT32_MAX || presentIndex == UINT32_MAX) {
        pInfo->graphicsIndex = UINT32_MAX;
        pInfo->presentIndex = UINT32_MAX;
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    /* Collect unique indices */
    if (graphicsIndex == presentIndex) {
        pInfo->count = 1;
        pInfo->indices[0] = graphicsIndex;
    } else {
        pInfo->count = 2;
        pInfo->indices[0] = graphicsIndex;
        pInfo->indices[1] = presentIndex;
    }

    pInfo->graphicsIndex = graphicsIndex;
    pInfo->presentIndex = presentIndex;
    return VK_SUCCESS;
}

static VkResult
getSurfaceInfo(VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkPhysicalDevice device, VkSurfaceKHR surface, VkUtilsSurfaceInfo* pInfo)
{
    VkResult result;

    result = pInstanceDispatchTable->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device,
        surface,
        &pInfo->capabilities
    );
    if (result != VK_SUCCESS) {
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    result = vkUtilsGetPhysicalDeviceSurfaceFormats(
        pInstanceDispatchTable->vkGetPhysicalDeviceSurfaceFormatsKHR,
        device,
        surface,
        &pInfo->formatCount,
        &pInfo->pFormats
    );
    if (result != VK_SUCCESS) {
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    result = vkUtilsGetPhysicalDeviceSurfacePresentModes(
        pInstanceDispatchTable->vkGetPhysicalDeviceSurfacePresentModesKHR,
        device,
        surface,
        &pInfo->presentModeCount,
        &pInfo->pPresentModes
    );
    if (result != VK_SUCCESS || pInfo->presentModeCount < 1) {
        free(pInfo->pPresentModes);
        pInfo->pPresentModes = NULL;
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    return VK_SUCCESS;
}

static VkResult
getDeviceInfo(VkInstance instance, VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkPhysicalDevice device, VkSurfaceKHR surface, VkUtilsGraphicsDeviceInfo* pInfo)
{
    VkResult result;

    result = getQueueFamiliesInfo(instance, pInstanceDispatchTable, device, &pInfo->queueFamilies);
    if (result != VK_SUCCESS) {
        return result;
    }

    result = getSurfaceInfo(pInstanceDispatchTable, device, surface, &pInfo->surface);
    if (result != VK_SUCCESS) {
        return result;
    }

    pInstanceDispatchTable->vkGetPhysicalDeviceProperties(device, &pInfo->properties);
    pInstanceDispatchTable->vkGetPhysicalDeviceMemoryProperties(device, &pInfo->memoryProperties);

    return VK_SUCCESS;
}

static VkBool32
isDeviceSuitable(VkInstance instance, VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkPhysicalDevice device, VkSurfaceKHR surface, VkUtilsGraphicsDeviceInfo* pInfo)
{
    VkResult result;

    if (!vkUtilsDeviceSupportsExtensions(pInstanceDispatchTable->vkEnumerateDeviceExtensionProperties, device, NULL, ARRAY_LENGTH(requiredExtensionNames), requiredExtensionNames)) {
        return VK_FALSE;
    }

    result = getDeviceInfo(instance, pInstanceDispatchTable, device, surface, pInfo);
    if (result != VK_SUCCESS) {
        return VK_FALSE;
    }

    return VK_TRUE;
}

static VkPhysicalDevice
selectPhysicalDevice(VkInstance instance, VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkSurfaceKHR surface, VkUtilsGraphicsDeviceInfo* pInfo)
{
    VkResult result;
    uint32_t deviceCount;
    VkPhysicalDevice *pDevices, device;

    result = vkUtilsGetPhysicalDevices(
        pInstanceDispatchTable->vkEnumeratePhysicalDevices,
        instance,
        &deviceCount,
        &pDevices
    );
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to get device list");
        return VK_NULL_HANDLE;
    }

    device = VK_NULL_HANDLE;
    for (uint32_t d = 0; d < deviceCount; d++) {
        if (isDeviceSuitable(instance, pInstanceDispatchTable, pDevices[d], surface, pInfo)) {
            device = pDevices[d];
            break;
        }
    }

    free(pDevices);
    return device;
}

VkBool32
vkUtilsDeviceSupportsExtensions(PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t requestedNameCount, const char** ppRequestedNames)
{
    VkResult result;
    uint32_t propertyCount;
    VkExtensionProperties* pProperties;
    VkBool32 found;

    result = vkUtilsGetDeviceExtensionProperties(vkEnumerateDeviceExtensionProperties, physicalDevice, pLayerName, &propertyCount, &pProperties);
    if (result != VK_SUCCESS || propertyCount < 1) {
        return VK_FALSE;
    }

    /* Enumerate requested extensions */
    for (uint32_t r = 0; r < requestedNameCount; r++) {
        /* Search properties for requested extension */
        found = VK_FALSE;
        for (uint32_t p = 0; p < propertyCount; p++) {
            if (strcmp(pProperties[p].extensionName, ppRequestedNames[r]) == 0) {
                found = VK_TRUE;
                break;
            }
        }

        if (!found) {
            free(pProperties);
            return VK_FALSE;
        }
    }

    free(pProperties);
    return VK_TRUE;
}

void
vkUtilsDestroyGraphicsDevice(VkDevice device, VkUtilsGraphicsDeviceInfo* pInfo)
{
    if (device == VK_NULL_HANDLE || pInfo == NULL) {
        return;
    }

    if (pInfo->surface.pPresentModes != NULL) {
        free(pInfo->surface.pPresentModes);
        pInfo->surface.pPresentModes = NULL;
        pInfo->surface.presentModeCount = 0;
    }

    if (pInfo->surface.pFormats != NULL) {
        free(pInfo->surface.pFormats);
        pInfo->surface.pFormats = NULL;
        pInfo->surface.formatCount = 0;
    }

    pInfo->dispatchTable.vkDestroyDevice(device, NULL);
}

VkResult
vkUtilsCreateGraphicsDevice(VkInstance instance, VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkSurfaceKHR surface, VkUtilsGraphicsDeviceInfo* pInfo, VkDevice* pDevice)
{
    VkResult result;
    VkPhysicalDevice physicalDevice;
    float queuePriority;
    VkDeviceQueueCreateInfo queueInfos[2], *pQueueInfo;
    VkPhysicalDeviceFeatures features;
    VkDevice device;
    VkUtilsDeviceDispatchTable* pDispatchTable;

    physicalDevice = selectPhysicalDevice(instance, pInstanceDispatchTable, surface, pInfo);
    if (physicalDevice == VK_NULL_HANDLE) {
        vkUtilsLogError(VK_ERROR_FEATURE_NOT_PRESENT, "failed to find a suitable physical device");
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    vkUtilsLogDebug("device name: %s", pInfo->properties.deviceName);
    vkUtilsLogDebug("device type: %s", vkUtilsGetPhysicalDeviceTypeString(pInfo->properties.deviceType));
    vkUtilsLogDebug("queue family indices: graphics=%u, present=%u", pInfo->queueFamilies.graphicsIndex, pInfo->queueFamilies.presentIndex);

    /* Create a queue for each unique queue family */
    queuePriority = 1.0f;
    for (uint32_t f = 0; f < pInfo->queueFamilies.count; f++) {
        pQueueInfo = &queueInfos[f];
        pQueueInfo->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        pQueueInfo->pNext = NULL;
        pQueueInfo->flags = 0;
        pQueueInfo->queueFamilyIndex = pInfo->queueFamilies.indices[f];
        pQueueInfo->queueCount = 1;
        pQueueInfo->pQueuePriorities = &queuePriority;
    }

    memzero(&features, sizeof(features));
    deviceInfo.queueCreateInfoCount = pInfo->queueFamilies.count;
    deviceInfo.pQueueCreateInfos = queueInfos;
    deviceInfo.pEnabledFeatures = &features;

    result = pInstanceDispatchTable->vkCreateDevice(physicalDevice, &deviceInfo, NULL, &device);
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to create logical device");
        return result;
    }

    /* Load symbols into dispatch table */
    pDispatchTable = &pInfo->dispatchTable;
    LOAD_DEVICE_PROC(vkGetDeviceQueue);
    LOAD_DEVICE_PROC(vkCreateRenderPass);
    LOAD_DEVICE_PROC(vkCreateSwapchainKHR);
    LOAD_DEVICE_PROC(vkGetSwapchainImagesKHR);
    LOAD_DEVICE_PROC(vkCreateImageView);
    LOAD_DEVICE_PROC(vkCreateFramebuffer);
    LOAD_DEVICE_PROC(vkDestroyFramebuffer);
    LOAD_DEVICE_PROC(vkDestroyImageView);
    LOAD_DEVICE_PROC(vkDestroyRenderPass);
    LOAD_DEVICE_PROC(vkDestroySwapchainKHR);
    LOAD_DEVICE_PROC(vkDestroyDevice);

    /* Get queue handles */
    if (pInfo->queueFamilies.count == 1) {
        pDispatchTable->vkGetDeviceQueue(device, pInfo->queueFamilies.graphicsIndex, 0, &pInfo->graphicsQueue);
        pInfo->presentQueue = pInfo->graphicsQueue;
    } else {
        pDispatchTable->vkGetDeviceQueue(device, pInfo->queueFamilies.graphicsIndex, 0, &pInfo->graphicsQueue);
        pDispatchTable->vkGetDeviceQueue(device, pInfo->queueFamilies.presentIndex, 0, &pInfo->presentQueue);
    }

    *pDevice = device;
    return VK_SUCCESS;
}
