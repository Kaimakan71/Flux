/*
 * Vulkan symbol loader.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/vkUtils.h"

PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = NULL;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = NULL;
PFN_vkCreateInstance vkCreateInstance = NULL;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;

VkResult
vkUtilsLoadGlobalSymbols(VkUtilsGetSymbolAddress getSymbolAddress)
{
    vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)getSymbolAddress(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)getSymbolAddress(VK_NULL_HANDLE, "vkEnumerateInstanceLayerProperties");
    vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)getSymbolAddress(VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
    vkCreateInstance = (PFN_vkCreateInstance)getSymbolAddress(VK_NULL_HANDLE, "vkCreateInstance");
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)getSymbolAddress(VK_NULL_HANDLE, "vkGetInstanceProcAddr");

    return VK_SUCCESS;
}
