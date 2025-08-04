/*
 * Vulkan API wrappers.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vkUtils.h"

#include <stdlib.h>
#include <string.h>

/*
 * NOTE: Using macros like this is stupid.
 * Quark should have some kind of templates.
 */

#define MAKE_WRAPPER(vkName, typeName, countName, pCountName, pDataName, ppDataName, ...) \
    VkResult result; \
    uint32_t countName; \
    typeName* pDataName; \
    \
    result = vkName(__VA_ARGS__, &countName, NULL); \
    if (result != VK_SUCCESS) { \
        return result; \
    } \
    \
    if (countName < 1) { \
        *pCountName = 0; \
        *ppDataName = NULL; \
        return VK_SUCCESS; \
    } \
    \
    pDataName = malloc(sizeof(*pDataName) * countName); \
    if (pDataName == NULL) { \
        return VK_ERROR_OUT_OF_HOST_MEMORY; \
    } \
    \
    result = vkName(__VA_ARGS__, &countName, pDataName); \
    if (result != VK_SUCCESS) { \
        free(pDataName); \
        return result; \
    } \
    \
    *pCountName = countName; \
    *ppDataName = pDataName; \
    return VK_SUCCESS;

#define MAKE_WRAPPER_NO_ARGS(vkName, typeName, countName, pCountName, pDataName, ppDataName) \
    VkResult result; \
    uint32_t countName; \
    typeName* pDataName; \
    \
    result = vkName(&countName, NULL); \
    if (result != VK_SUCCESS) { \
        return result; \
    } \
    \
    if (countName < 1) { \
        *pCountName = 0; \
        *ppDataName = NULL; \
        return VK_SUCCESS; \
    } \
    \
    pDataName = malloc(sizeof(*pDataName) * countName); \
    if (pDataName == NULL) { \
        return VK_ERROR_OUT_OF_HOST_MEMORY; \
    } \
    \
    result = vkName(&countName, pDataName); \
    if (result != VK_SUCCESS) { \
        free(pDataName); \
        return result; \
    } \
    \
    *pCountName = countName; \
    *ppDataName = pDataName; \
    return VK_SUCCESS;

VkResult
vkUtilsGetInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties** ppProperties)
{
    MAKE_WRAPPER_NO_ARGS(vkEnumerateInstanceLayerProperties, VkLayerProperties, propertyCount, pPropertyCount, pProperties, ppProperties)
}

VkResult
vkUtilsGetInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties** ppProperties)
{
    MAKE_WRAPPER(vkEnumerateInstanceExtensionProperties, VkExtensionProperties, propertyCount, pPropertyCount, pProperties, ppProperties, pLayerName)
}
