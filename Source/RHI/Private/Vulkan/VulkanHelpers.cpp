/*
 * Vulkan helpers.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cstring>
#include <new>
#include "VulkanHelpers.hpp"
#include "VulkanLoader.hpp"

namespace Flux::VulkanHelpers {

VkResult getInstanceVersion(uint32_t *version) {
    VkResult result;

    /* vkEnumerateInstanceVersion() was added in Vulkan 1.1 */
    if (VulkanLoader::globalDispatch.vkEnumerateInstanceVersion == nullptr) {
        *version = VK_API_VERSION_1_0;
        return VK_SUCCESS;
    }

    result = VulkanLoader::globalDispatch.vkEnumerateInstanceVersion(version);
    if (result != VK_SUCCESS) {
        return result;
    }

    return VK_SUCCESS;
}

VkResult getInstanceLayerProperties(uint32_t *propertyCountOut, VkLayerProperties **propertiesOut) {
    VkResult result;
    uint32_t propertyCount;
    VkLayerProperties *properties;

    result = VulkanLoader::globalDispatch.vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (propertyCount < 1) {
        *propertyCountOut = 0;
        *propertiesOut = nullptr;
        return VK_SUCCESS;
    }

    properties = new(std::nothrow) VkLayerProperties[propertyCount];
    if (properties == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    result = VulkanLoader::globalDispatch.vkEnumerateInstanceLayerProperties(&propertyCount, properties);
    if (result != VK_SUCCESS) {
        delete[] properties;
        return result;
    }

    *propertyCountOut = propertyCount;
    *propertiesOut = properties;
    return VK_SUCCESS;
}

VkResult filterInstanceLayerNames(uint32_t requestedNameCount, const char **requestedNames, uint32_t *availableNameCountOut, const char ***availableNamesOut) {
    VkResult result;
    uint32_t propertyCount, availableNameCount;
    VkLayerProperties *properties;
    const char **availableNames;

    result = getInstanceLayerProperties(&propertyCount, &properties);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (propertyCount < 1)  {
        *availableNameCountOut = 0;
        *availableNamesOut = nullptr;
        return VK_SUCCESS;
    }

    availableNames = new(std::nothrow) const char*[requestedNameCount];
    if (availableNames == nullptr) {
        delete[] properties;
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    /* Find the intersection between available and requested layers */
    availableNameCount = 0;
    for (uint32_t r = 0; r < requestedNameCount; r++)  {
        for (uint32_t p = 0; p < propertyCount; p++) {
            if (strcmp(properties[p].layerName, requestedNames[r]) == 0) {
                availableNames[availableNameCount++] = requestedNames[r];
                break;
            }
        }
    }

    delete[] properties;
    *availableNameCountOut = availableNameCount;
    *availableNamesOut = availableNames;
    return VK_SUCCESS;
}

}
