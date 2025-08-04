/*
 * Vulkan utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _RHI_VKUTILS_H
#define _RHI_VKUTILS_H

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(VK_UTILS_DEBUG_ENABLED)
#define vkUtilsLogDebug(...) __vkUtilsLogDebug(__FUNCTION__, __VA_ARGS__)
#else
#define vkUtilsLogDebug(...)
#endif
#define vkUtilsLogError(...) __vkUtilsLogError(__FUNCTION__, __VA_ARGS__)

typedef struct {
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkDestroyInstance vkDestroyInstance;
} VkUtilsInstanceDispatchTable;

#if defined(__cplusplus)
extern "C" {
#endif

/* Vulkan 1.0+ */
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

/* Vulkan 1.1+ */
extern PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;

void __vkUtilsLogDebug(const char* pFunctionName, const char* pFormat, ...);
void __vkUtilsLogError(const char* pFunctionName, VkResult result, const char* pFormat, ...);

const char* vkUtilsGetResultName(VkResult result);
const char* vkUtilsGetResultDescription(VkResult result);
const char* vkUtilsGetPhysicalDeviceTypeString(VkPhysicalDeviceType type);
const char* vkUtilsGetPresentModeString(VkPresentModeKHR mode);

typedef void* (*VkUtilsGetSymbolAddress)(VkInstance instance, const char* pName);
VkResult vkUtilsLoadGlobalSymbols(VkUtilsGetSymbolAddress getSymbolAddress);

VkResult vkUtilsGetInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties** ppProperties);
VkResult vkUtilsGetInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties** ppProperties);

VkResult vkUtilsGetInstanceVersion(uint32_t* pApiVersion);
VkResult vkUtilsFilterInstanceLayerNames(uint32_t requestedNameCount, const char** ppRequestedNames, uint32_t* pAvailableNameCount, const char*** pppAvailableNames);
VkResult vkUtilsCreateInstance(const char* pApplicationName, VkInstance* pInstance, VkUtilsInstanceDispatchTable* pDispatchTable);

#if defined(__cplusplus)
}
#endif

#endif /* !_RHI_VKUTILS_H */
