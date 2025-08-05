/*
 * Vulkan utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _RHI_VK_UTILS_H
#define _RHI_VK_UTILS_H

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if !defined(ARRAY_LENGTH)
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))
#endif

#if !defined(memzero)
#define memzero(data, length) memset((data), 0, (length))
#endif

#if defined(VK_UTILS_DEBUG_ENABLED)
#define vkUtilsLogDebug(...) __vkUtilsLogDebug(__FUNCTION__, __VA_ARGS__)
#else
#define vkUtilsLogDebug(...)
#endif
#define vkUtilsLogError(...) __vkUtilsLogError(__FUNCTION__, __VA_ARGS__)

#define VK_UTILS_MAX_QUEUE_FAMILY_COUNT 2

typedef struct {
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
    PFN_vkCreateDevice vkCreateDevice;
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
    PFN_vkDestroyInstance vkDestroyInstance;
} VkUtilsInstanceDispatchTable;

typedef struct {
    PFN_vkGetDeviceQueue vkGetDeviceQueue;
    PFN_vkCreateRenderPass vkCreateRenderPass;
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkCreateFramebuffer vkCreateFramebuffer;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkDestroyRenderPass vkDestroyRenderPass;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkDestroyDevice vkDestroyDevice;
} VkUtilsDeviceDispatchTable;

typedef struct {
    uint32_t count;
    uint32_t indices[VK_UTILS_MAX_QUEUE_FAMILY_COUNT];

    uint32_t graphicsIndex;
    uint32_t presentIndex;
} VkUtilsQueueFamiliesInfo;

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;

    uint32_t formatCount;
    VkSurfaceFormatKHR* pFormats;

    uint32_t presentModeCount;
    VkPresentModeKHR* pPresentModes;
} VkUtilsSurfaceInfo;

typedef struct {
    uint32_t imageCount;
    VkFormat imageFormat;
    VkExtent2D imageExtent;

    VkImageView* pImageViews;
    VkFramebuffer* pFramebuffers;
} VkUtilsSwapchainInfo;

typedef struct {
    VkUtilsDeviceDispatchTable dispatchTable;

    VkUtilsQueueFamiliesInfo queueFamilies;
    VkUtilsSurfaceInfo surface;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    VkUtilsSwapchainInfo swapchain;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
} VkUtilsGraphicsDeviceInfo;

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
VkResult vkUtilsGetDeviceExtensionProperties(PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties** ppProperties);
VkResult vkUtilsGetPhysicalDeviceQueueFamilyProperties(PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkQueueFamilyProperties** ppProperties);
VkResult vkUtilsGetPhysicalDeviceSurfaceFormats(PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pFormatCount, VkSurfaceFormatKHR** ppFormats);
VkResult vkUtilsGetPhysicalDeviceSurfacePresentModes(PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pModeCount, VkPresentModeKHR** ppModes);
VkResult vkUtilsGetPhysicalDevices(PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices, VkInstance instance, uint32_t* pDeviceCount, VkPhysicalDevice** ppDevices);
VkResult vkUtilsGetSwapchainImages(PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pImageCount, VkImage** ppImages);

VkResult vkUtilsGetInstanceVersion(uint32_t* pApiVersion);
VkResult vkUtilsFilterInstanceLayerNames(uint32_t requestedNameCount, const char** ppRequestedNames, uint32_t* pAvailableNameCount, const char*** pppAvailableNames);
VkResult vkUtilsCreateInstance(const char* pApplicationName, VkInstance* pInstance, VkUtilsInstanceDispatchTable* pDispatchTable);

VkBool32 vkUtilsDeviceSupportsExtensions(PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t requestedNameCount, const char** ppRequestedNames);
VkResult vkUtilsCreateGraphicsDevice(VkInstance instance, VkUtilsInstanceDispatchTable* pInstanceDispatchTable, VkSurfaceKHR surface, VkUtilsGraphicsDeviceInfo* pInfo, VkDevice* pDevice);
void vkUtilsDestroyGraphicsDevice(VkDevice device, VkUtilsGraphicsDeviceInfo* pInfo);

VkResult vkUtilsCreateSwapchain(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkSurfaceKHR surface, GLFWwindow* window, VkUtilsGraphicsDeviceInfo* pDeviceInfo, VkSwapchainKHR* pSwapchain);
VkResult vkUtilsCreateSwapchainFramebuffers(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkRenderPass renderPass, VkUtilsSwapchainInfo* pInfo);
void vkUtilsDestroySwapchainFramebuffers(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkUtilsSwapchainInfo* pInfo);
void vkUtilsDestroySwapchain(VkDevice device, VkUtilsDeviceDispatchTable* pDeviceDispatchTable, VkSwapchainKHR swapchain, VkUtilsSwapchainInfo* pInfo);

#if defined(__cplusplus)
}
#endif

#endif /* !_RHI_VK_UTILS_H */
