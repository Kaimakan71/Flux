/*
 * Vulkan loader.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "VulkanLoader.hpp"

#define LOAD_SYMBOL_GLOBAL(name)   globalDispatch.name = reinterpret_cast<PFN_##name>(globalDispatch.vkGetInstanceProcAddr(VK_NULL_HANDLE, #name));
#define LOAD_SYMBOL_INSTANCE(name) dispatch->name = reinterpret_cast<PFN_##name>(dispatch->vkGetInstanceProcAddr(instance, #name));
#define LOAD_SYMBOL_DEVICE(name)   deviceDispatch->name = reinterpret_cast<PFN_##name>(deviceDispatch->vkGetDeviceProcAddr(device, #name));

namespace Flux::VulkanLoader {

VulkanGlobalDispatch globalDispatch;

Status loadGlobalSymbols(GetSymbolAddress load) {
    FLUX_LOG_DEBUG("Loading global symbols...");

    /* Use global vkGetInstanceProcAddr() to load global symbols */
    globalDispatch.vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(load(VK_NULL_HANDLE, "vkGetInstanceProcAddr"));
    if (globalDispatch.vkGetInstanceProcAddr == nullptr) {
        FLUX_LOG_ERROR("Failed to load global symbol \"vkGetInstanceProcAddr\"");
        return Status::incompatibleDriver;
    }

    ENUMERATE_VULKAN_SYMBOLS_GLOBAL(LOAD_SYMBOL_GLOBAL);

    return Status::success;
}

void loadInstanceSymbols(VkInstance instance, VulkanInstanceDispatch *dispatch) {
    FLUX_LOG_DEBUG("Loading instance symbols...");

    /* Use global vkGetInstanceProcAddr() to load an instance-specific version */
    dispatch->vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(globalDispatch.vkGetInstanceProcAddr(instance, "vkGetInstanceProcAddr"));
    if (dispatch->vkGetInstanceProcAddr == nullptr) {
        FLUX_LOG_WARNING("Failed to load instance symbol \"vkGetInstanceProcAddr\", falling back to global version");
        dispatch->vkGetInstanceProcAddr = globalDispatch.vkGetInstanceProcAddr;
    }

    ENUMERATE_VULKAN_SYMBOLS_INSTANCE(LOAD_SYMBOL_INSTANCE);
}

void loadDeviceSymbols(VulkanInstanceDispatch *instanceDispatch, VkDevice device, VulkanDeviceDispatch *deviceDispatch) {
    FLUX_LOG_DEBUG("Loading device symbols...");

    /* Use instance vkGetDeviceProcAddr() to load a device-specific version */
    deviceDispatch->vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(instanceDispatch->vkGetDeviceProcAddr(device, "vkGetDeviceProcAddr"));
    if (deviceDispatch->vkGetDeviceProcAddr == nullptr) {
        FLUX_LOG_WARNING("Failed to load device symbol \"vkGetDeviceProcAddr\", falling back to instance version");
        deviceDispatch->vkGetDeviceProcAddr = instanceDispatch->vkGetDeviceProcAddr;
    }

    ENUMERATE_VULKAN_SYMBOLS_DEVICE(LOAD_SYMBOL_DEVICE);
}

}
