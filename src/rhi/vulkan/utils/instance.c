/*
 * Vulkan instance creation helper.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vkUtils.h"

#include <stdlib.h>
#include <string.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

#define LOAD_INSTANCE_PROC(name) pDispatchTable->name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);

static const char* optionalLayerNames[] = {
    "VK_LAYER_KHRONOS_validation"
};

static VkApplicationInfo applicationInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = NULL,
    .pApplicationName = "vkUtils application",
    .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
    .pEngineName = "Flux",
    .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
    .apiVersion = VK_API_VERSION_1_4
};

static VkInstanceCreateInfo instanceInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .pApplicationInfo = &applicationInfo,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .enabledExtensionCount = 0,
    .ppEnabledExtensionNames = NULL
};

VkResult
vkUtilsGetInstanceVersion(uint32_t* pApiVersion)
{
    VkResult result;
    uint32_t apiVersion;

    /* vkEnumerateInstanceVersion() was added in Vulkan 1.1 */
    if (vkEnumerateInstanceVersion == NULL) {
        *pApiVersion = VK_API_VERSION_1_0;
        return VK_SUCCESS;
    }

    result = vkEnumerateInstanceVersion(&apiVersion);
    if (result != VK_SUCCESS) {
        return result;
    }

    *pApiVersion = apiVersion;
    return result;
}

VkResult
vkUtilsFilterInstanceLayerNames(uint32_t requestedNameCount, const char** ppRequestedNames, uint32_t* pAvailableNameCount, const char*** pppAvailableNames)
{
    VkResult result;
    uint32_t propertyCount, availableNameCount;
    VkLayerProperties* pProperties;
    const char** ppAvailableNames;

    result = vkUtilsGetInstanceLayerProperties(&propertyCount, &pProperties);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (propertyCount < 1) {
        *pAvailableNameCount = 0;
        *pppAvailableNames = NULL;
        return VK_SUCCESS;
    }

    ppAvailableNames = malloc(sizeof(const char*) * requestedNameCount);
    if (ppAvailableNames == NULL) {
        free(pProperties);
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    /* Make a list of the available requested layers */
    availableNameCount = 0;
    for (uint32_t r = 0; r < requestedNameCount; r++) {
        for (uint32_t p = 0; p < propertyCount; p++) {
            if (strcmp(pProperties[p].layerName, ppRequestedNames[r]) == 0) {
                ppAvailableNames[availableNameCount++] = ppRequestedNames[r];
                break;
            }
        }
    }

    free(pProperties);
    *pAvailableNameCount = availableNameCount;
    *pppAvailableNames = ppAvailableNames;
    return VK_SUCCESS;
}

VkResult
vkUtilsCreateInstance(const char* pApplicationName, VkInstance* pInstance, VkUtilsInstanceDispatchTable* pDispatchTable)
{
    VkResult result;
    uint32_t apiVersion, availableLayerCount, glfwExtensionCount;
    const char** availableLayerNames;
    const char** glfwExtensionNames;
    VkInstance instance;

    applicationInfo.pApplicationName = pApplicationName;

    /* Vulkan 1.0 doesn't like higher apiVersion values */
    if (vkUtilsGetInstanceVersion(&apiVersion) == VK_SUCCESS && apiVersion == VK_API_VERSION_1_0) {
        applicationInfo.apiVersion = VK_API_VERSION_1_0;
    } else {
        applicationInfo.apiVersion = VK_API_VERSION_1_4;
    }

    /* Try to enable optional layers */
    result = vkUtilsFilterInstanceLayerNames(
        ARRAY_LENGTH(optionalLayerNames),
        optionalLayerNames,
        &availableLayerCount,
        &availableLayerNames
    );
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to filter optional layer names");
        availableLayerCount = 0;
        availableLayerNames = NULL;
    }
    instanceInfo.enabledLayerCount = availableLayerCount;
    instanceInfo.ppEnabledLayerNames = availableLayerNames;
    vkUtilsLogDebug("%u out of %u optional layers are available", availableLayerCount, ARRAY_LENGTH(optionalLayerNames));

    /* Enable extensions required by GLFW */
    glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensionNames == NULL || glfwExtensionCount < 1) {
        /* TODO: Print GLFW error code/message here */
        vkUtilsLogError(VK_ERROR_UNKNOWN, "failed to get required extension names");
        return VK_ERROR_UNKNOWN;
    }
    instanceInfo.enabledExtensionCount = glfwExtensionCount;
    instanceInfo.ppEnabledExtensionNames = glfwExtensionNames;

    result = vkCreateInstance(&instanceInfo, NULL, &instance);

    /* Need to do cleanup either way */
    if (availableLayerNames != NULL) {
        free(availableLayerNames);
    }

    /* Now handle errors */
    if (result != VK_SUCCESS) {
        vkUtilsLogError(result, "failed to create instance");
        return result;
    }

    /* Load symbols into dispatch table */
    LOAD_INSTANCE_PROC(vkEnumeratePhysicalDevices);
    LOAD_INSTANCE_PROC(vkEnumerateDeviceExtensionProperties);
    LOAD_INSTANCE_PROC(vkGetPhysicalDeviceQueueFamilyProperties);
    LOAD_INSTANCE_PROC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    LOAD_INSTANCE_PROC(vkGetPhysicalDeviceSurfaceFormatsKHR);
    LOAD_INSTANCE_PROC(vkGetPhysicalDeviceSurfacePresentModesKHR);
    LOAD_INSTANCE_PROC(vkGetPhysicalDeviceProperties);
    LOAD_INSTANCE_PROC(vkGetPhysicalDeviceMemoryProperties);
    LOAD_INSTANCE_PROC(vkCreateDevice);
    LOAD_INSTANCE_PROC(vkGetDeviceProcAddr);
    LOAD_INSTANCE_PROC(vkDestroySurfaceKHR);
    LOAD_INSTANCE_PROC(vkDestroyInstance);

    *pInstance = instance;
    return VK_SUCCESS;
}
