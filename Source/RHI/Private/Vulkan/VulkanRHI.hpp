/*
 * Vulkan RHI.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "RHI.hpp"
#include "VulkanDispatch.hpp"

#define MAX_QUEUE_FAMILY_COUNT 2

typedef struct {
    uint32_t count;
    uint32_t indices[MAX_QUEUE_FAMILY_COUNT];

    uint32_t graphicsIndex;
    uint32_t presentIndex;
} VulkanDeviceQueueFamiliesInfo;

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;

    uint32_t formatCount;
    VkSurfaceFormatKHR *formats;

    uint32_t presentModeCount;
    VkPresentModeKHR *presentModes;
} VulkanDeviceSurfaceInfo;

typedef struct {
    VulkanDeviceQueueFamiliesInfo queueFamilies;
    VulkanDeviceSurfaceInfo surface;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
} VulkanDeviceInfo;

namespace Flux {

class VulkanRHI: public RHI {
private:

    VkResult createInstance(void);

public:

    VkInstance instance;
    VulkanInstanceDispatch dispatch;

    static VkResult getInstanceVersion(uint32_t *version);

    static VkResult getInstanceLayerProperties(uint32_t *propertyCount, VkLayerProperties **properties);

    static VkResult filterInstanceLayerNames(uint32_t requestedNameCount, const char **requestedNames, uint32_t *availableNameCount, const char ***availableNames);

    VkResult getPhysicalDevices(uint32_t *deviceCount, VkPhysicalDevice **devices);

    VkBool32 deviceSupportsExtensions(VkPhysicalDevice device, const char *layerName, uint32_t requestedNameCount, const char **requestedNames);

    VkResult getPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice device, uint32_t *propertyCount, VkQueueFamilyProperties **properties);

    VkResult getPhysicalDeviceSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t *formatCount, VkSurfaceFormatKHR **formats);

    VkResult getPhysicalDeviceSurfacePresentModes(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t *modeCount, VkPresentModeKHR **modes);

    VkResult getDeviceQueueFamiliesInfo(VkPhysicalDevice device, VulkanDeviceQueueFamiliesInfo *info);

    VkResult getDeviceSurfaceInfo(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanDeviceSurfaceInfo *info);

    VkResult getDeviceInfo(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanDeviceInfo *info);

    virtual Status initialize(void);

    virtual Status createDevice(Window &window, RHIDevice **device);

    virtual void shutdown(void);

};

}
