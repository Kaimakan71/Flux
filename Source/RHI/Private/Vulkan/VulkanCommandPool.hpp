/*
 * Vulkan RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanDevice.hpp"

namespace Flux {

class VulkanCommandPool: public RHICommandPool {
private:

    VulkanDevice *device;

    VkCommandPool pool;

public:

    Status create(VulkanDevice *device);

    virtual void destroy(void);

};

}
