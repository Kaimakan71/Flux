/*
 * Vulkan helpers.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanDispatch.hpp"

namespace Flux::VulkanHelpers {

VkResult getInstanceVersion(uint32_t *version);

VkResult getInstanceLayerProperties(uint32_t *propertyCount, VkLayerProperties **properties);

VkResult filterInstanceLayerNames(uint32_t requestedNameCount, const char **requestedNames, uint32_t *availableNameCount, const char ***availableNames);

}
