/*
 * Vulkan dispatch.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#define ENUMERATE_VULKAN_SYMBOLS_GLOBAL(macro) \
    macro(vkEnumerateInstanceVersion)          \
    macro(vkEnumerateInstanceLayerProperties)  \
    macro(vkCreateInstance)

#define ENUMERATE_VULKAN_SYMBOLS_INSTANCE(macro)     \
    macro(vkEnumeratePhysicalDevices)                \
    macro(vkEnumerateDeviceExtensionProperties)      \
    macro(vkGetPhysicalDeviceQueueFamilyProperties)  \
    macro(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
    macro(vkGetPhysicalDeviceSurfaceFormatsKHR)      \
    macro(vkGetPhysicalDeviceSurfacePresentModesKHR) \
    macro(vkGetPhysicalDeviceProperties)             \
    macro(vkGetPhysicalDeviceMemoryProperties)       \
    macro(vkCreateDevice)                            \
    macro(vkGetDeviceProcAddr)                       \
    macro(vkDestroySurfaceKHR)                       \
    macro(vkDestroyInstance)

#define ENUMERATE_VULKAN_SYMBOLS_DEVICE(macro) \
    macro(vkGetDeviceQueue)                    \
    macro(vkCreateSwapchainKHR)                \
    macro(vkGetSwapchainImagesKHR)             \
    macro(vkCreateRenderPass)                  \
    macro(vkCreateImageView)                   \
    macro(vkCreateFramebuffer)                 \
    macro(vkCreatePipelineLayout)              \
    macro(vkCreateShaderModule)                \
    macro(vkCreateGraphicsPipelines)           \
    macro(vkCreateCommandPool)                 \
    macro(vkDestroyCommandPool)                \
    macro(vkDestroyShaderModule)               \
    macro(vkDestroyPipeline)                   \
    macro(vkDestroyPipelineLayout)             \
    macro(vkDestroyFramebuffer)                \
    macro(vkDestroyImageView)                  \
    macro(vkDestroyRenderPass)                 \
    macro(vkDestroySwapchainKHR)               \
    macro(vkDestroyDevice)

#define _DEFINE_MEMBER(name) PFN_##name name;

namespace Flux {

typedef struct {
    /* Used to load remaining symbols */
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

    ENUMERATE_VULKAN_SYMBOLS_GLOBAL(_DEFINE_MEMBER);
} VulkanGlobalDispatch;

typedef struct {
    /* Used to load remaining symbols */
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

    ENUMERATE_VULKAN_SYMBOLS_INSTANCE(_DEFINE_MEMBER);
} VulkanInstanceDispatch;

typedef struct {
    /* Used to load remaining symbols */
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

    ENUMERATE_VULKAN_SYMBOLS_DEVICE(_DEFINE_MEMBER);
} VulkanDeviceDispatch;

}
