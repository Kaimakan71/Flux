/*
 * Vulkan loader.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanDispatch.hpp"
#include "Status.hpp"

namespace Flux::VulkanLoader {

extern VulkanGlobalDispatch globalDispatch;

typedef PFN_vkVoidFunction (*GetSymbolAddress)(VkInstance instance, const char *name);

Status loadGlobalSymbols(GetSymbolAddress load);

void loadInstanceSymbols(VkInstance instance, VulkanInstanceDispatch *dispatch);

void loadDeviceSymbols(VulkanInstanceDispatch *instanceDispatch, VkDevice device, VulkanDeviceDispatch *deviceDispatch);

}
