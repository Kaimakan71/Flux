/*
 * VkResult utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include "Status.hpp"

namespace Flux::VulkanResult {

Status getStatus(VkResult result);

const char *getName(VkResult result);

const char *getDescription(VkResult result);

}
