/*
 * Vulkan RHI.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstring>
#include "Log.hpp"
#include "VulkanDevice.hpp"
#include "VulkanHelpers.hpp"
#include "VulkanLoader.hpp"
#include "VulkanLog.hpp"
#include "VulkanResult.hpp"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

static bool globalInitialized = false;

namespace Flux {

void VulkanRHI::shutdown(void) {
    FLUX_LOG_DEBUG("Shutting down Vulkan RHI...");

    if (this->instance != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying instance...");
        this->dispatch.vkDestroyInstance(this->instance, nullptr);
        this->instance = VK_NULL_HANDLE;
    }
}

VkResult VulkanRHI::createInstance(void) {
    VkResult result;
    uint32_t apiVersion, availableLayerCount, glfwExtensionCount;
    const char **availableLayerNames, **glfwExtensionNames;

    static VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Flux",
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .pEngineName = "Flux",
        .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    static VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
    };

    /* TODO: Only request validation for debug builds */
    static const char *optionalLayerNames[] = {
        "VK_LAYER_KHRONOS_validation",
    };

    result = VulkanHelpers::getInstanceVersion(&apiVersion);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_WARNING(result, "Failed to get instance version, assuming 1.0");
        apiVersion = VK_API_VERSION_1_0;
    }

    /* Vulkan 1.0 doesn't like higher apiVersion values */
    if (apiVersion == VK_API_VERSION_1_0) {
        FLUX_LOG_DEBUG("Requesting API version 1.0");
        applicationInfo.apiVersion = VK_API_VERSION_1_0;
    } else {
        FLUX_LOG_DEBUG("Requesting API version 1.4");
        applicationInfo.apiVersion = VK_API_VERSION_1_4;
    }

    /* Try to enable optional layers */
    result = VulkanHelpers::filterInstanceLayerNames(ARRAY_SIZE(optionalLayerNames), optionalLayerNames, &availableLayerCount, &availableLayerNames);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_WARNING(result, "Failed to filter optional instance layer names, requesting none");
        availableLayerCount = 0;
        availableLayerNames = nullptr;
    }
    createInfo.enabledLayerCount = availableLayerCount;
    createInfo.ppEnabledLayerNames = availableLayerNames;

    /* Enable extensions required by GLFW */
    glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensionNames == nullptr || glfwExtensionCount < 1) {
        FLUX_LOG_ERROR("Failed to get GLFW-required instance extension names");
        delete[] availableLayerNames;
        return VK_ERROR_INCOMPATIBLE_DRIVER;
    }
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensionNames;

    FLUX_LOG_DEBUG("Creating instance...");
    result = VulkanLoader::globalDispatch.vkCreateInstance(&createInfo, nullptr, &this->instance);
    delete[] availableLayerNames;
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create instance");
        return result;
    }

    VulkanLoader::loadInstanceSymbols(instance, &this->dispatch);

    return VK_SUCCESS;
}

Status VulkanRHI::initialize(void) {
    Status status;
    VkResult result;

    FLUX_LOG_DEBUG("Initializing Vulkan RHI...");

    if (!globalInitialized) {
        status = VulkanLoader::loadGlobalSymbols(glfwGetInstanceProcAddress);
        if (status != Status::success) {
            return status;
        }

        globalInitialized = true;
    }

    result = this->createInstance();
    if (result != VK_SUCCESS) {
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

VkResult VulkanRHI::getPhysicalDevices(uint32_t *deviceCountOut, VkPhysicalDevice **devicesOut) {
    VkResult result;
    uint32_t deviceCount;
    VkPhysicalDevice *devices;

    result = this->dispatch.vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (deviceCount < 1) {
        *deviceCountOut = 0;
        *devicesOut = nullptr;
        return VK_SUCCESS;
    }

    devices = new(std::nothrow) VkPhysicalDevice[deviceCount];
    if (devices == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    result = this->dispatch.vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices);
    if (result != VK_SUCCESS) {
        delete[] devices;
        return result;
    }

    *deviceCountOut = deviceCount;
    *devicesOut = devices;
    return VK_SUCCESS;
}

VkBool32 VulkanRHI::deviceSupportsExtensions(VkPhysicalDevice device, const char *layerName, uint32_t requestedNameCount, const char **requestedNames) {
    VkResult result;
    uint32_t propertyCount;
    VkExtensionProperties *properties;
    VkBool32 found;

    result = this->dispatch.vkEnumerateDeviceExtensionProperties(device, layerName, &propertyCount, nullptr);
    if (result != VK_SUCCESS) {
        return VK_FALSE;
    }

    /* Fast path */
    if (propertyCount < requestedNameCount) {
        return VK_FALSE;
    }

    properties = new(std::nothrow) VkExtensionProperties[propertyCount];
    if (properties == nullptr) {
        return VK_FALSE;
    }

    result = this->dispatch.vkEnumerateDeviceExtensionProperties(device, layerName, &propertyCount, properties);
    if (result != VK_SUCCESS) {
        delete[] properties;
        return VK_FALSE;
    }

    /* Enumerate requested extensions */
    for (uint32_t r = 0; r < requestedNameCount; r++) {
        /* Search properties for requested extension */
        found = VK_FALSE;
        for (uint32_t p = 0; p < propertyCount; p++) {
            if (strcmp(properties[p].extensionName, requestedNames[r]) == 0) {
                found = VK_TRUE;
                break;
            }
        }

        if (!found) {
            delete[] properties;
            return VK_FALSE;
        }
    }

    delete[] properties;
    return VK_TRUE;
}

VkResult VulkanRHI::getPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice device, uint32_t *propertyCountOut, VkQueueFamilyProperties **propertiesOut) {
    uint32_t propertyCount;
    VkQueueFamilyProperties *properties;

    this->dispatch.vkGetPhysicalDeviceQueueFamilyProperties(device, &propertyCount, nullptr);

    if (propertyCount < 1) {
        *propertyCountOut = 0;
        *propertiesOut = nullptr;
        return VK_SUCCESS;
    }

    properties = new(std::nothrow) VkQueueFamilyProperties[propertyCount];
    if (properties == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    this->dispatch.vkGetPhysicalDeviceQueueFamilyProperties(device, &propertyCount, properties);

    *propertyCountOut = propertyCount;
    *propertiesOut = properties;
    return VK_SUCCESS;
}

VkResult VulkanRHI::getPhysicalDeviceSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t *formatCountOut, VkSurfaceFormatKHR **formatsOut) {
    VkResult result;
    uint32_t formatCount;
    VkSurfaceFormatKHR *formats;

    result = this->dispatch.vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (formatCount < 1) {
        *formatCountOut = 0;
        *formatsOut = nullptr;
        return VK_SUCCESS;
    }

    formats = new(std::nothrow) VkSurfaceFormatKHR[formatCount];
    if (formats == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    result = this->dispatch.vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats);
    if (result != VK_SUCCESS) {
        delete[] formats;
        return result;
    }

    *formatCountOut = formatCount;
    *formatsOut = formats;
    return VK_SUCCESS;
}

VkResult VulkanRHI::getPhysicalDeviceSurfacePresentModes(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t *modeCountOut, VkPresentModeKHR **modesOut) {
    VkResult result;
    uint32_t modeCount;
    VkPresentModeKHR *modes;

    result = this->dispatch.vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (modeCount < 1) {
        *modeCountOut = 0;
        *modesOut = nullptr;
        return VK_SUCCESS;
    }

    modes = new(std::nothrow) VkPresentModeKHR[modeCount];
    if (modes == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    result = this->dispatch.vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, modes);
    if (result != VK_SUCCESS) {
        delete[] modes;
        return result;
    }

    *modeCountOut = modeCount;
    *modesOut = modes;
    return VK_SUCCESS;
}

VkResult VulkanRHI::getDeviceQueueFamiliesInfo(VkPhysicalDevice device, VulkanDeviceQueueFamiliesInfo *info) {
    VkResult result;
    uint32_t propertyCount, graphicsIndex, presentIndex;
    VkQueueFamilyProperties *properties;

    result = this->getPhysicalDeviceQueueFamilyProperties(device, &propertyCount, &properties);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to get physical device queue family properties");
        return result;
    }

    graphicsIndex = UINT32_MAX;
    presentIndex = UINT32_MAX;
    for (uint32_t p = 0; p < propertyCount; p++) {
        if (properties[p].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsIndex = p;
        }

        if (glfwGetPhysicalDevicePresentationSupport(instance, device, p) == GLFW_TRUE) {
            presentIndex = p;
        }

        if (graphicsIndex != UINT32_MAX && presentIndex != UINT32_MAX) {
            break;
        }
    }
    delete[] properties;

    if (graphicsIndex == UINT32_MAX || presentIndex == UINT32_MAX) {
        info->graphicsIndex = UINT32_MAX;
        info->presentIndex = UINT32_MAX;
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    /* Collect unique indices */
    if (graphicsIndex == presentIndex) {
        info->count = 1;
        info->indices[0] = graphicsIndex;
    } else {
        info->count = 2;
        info->indices[0] = graphicsIndex;
        info->indices[1] = presentIndex;
    }

    info->graphicsIndex = graphicsIndex;
    info->presentIndex = presentIndex;
    return VK_SUCCESS;
}

VkResult VulkanRHI::getDeviceSurfaceInfo(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanDeviceSurfaceInfo *info) {
    VkResult result;

    result = this->dispatch.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &info->capabilities);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to get physical device surface capabilities");
        return result;
    }

    result = this->getPhysicalDeviceSurfaceFormats(device, surface, &info->formatCount, &info->formats);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to get physical device surface formats");
        return result;
    }

    result = this->getPhysicalDeviceSurfacePresentModes(device, surface, &info->presentModeCount, &info->presentModes);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to get physical device surface present modes");
        delete[] info->formats;
        return result;
    }

    return VK_SUCCESS;
}

VkResult VulkanRHI::getDeviceInfo(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanDeviceInfo *info) {
    VkResult result;

    result = this->getDeviceQueueFamiliesInfo(device, &info->queueFamilies);
    if (result != VK_SUCCESS) {
        return result;
    }

    result = this->getDeviceSurfaceInfo(device, surface, &info->surface);
    if (result != VK_SUCCESS) {
        return result;
    }

    this->dispatch.vkGetPhysicalDeviceProperties(device, &info->properties);
    this->dispatch.vkGetPhysicalDeviceMemoryProperties(device, &info->memoryProperties);

    return VK_SUCCESS;
}

Status VulkanRHI::createDevice(Window window, RHIDevice **deviceOut) {
    Status status;
    VulkanDevice *device;

    device = new(std::nothrow) VulkanDevice();
    if (device == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = device->create(this, window);
    if (status != Status::success) {
        delete device;
        return status;
    }

    *deviceOut = device;
    return Status::success;
}

}
