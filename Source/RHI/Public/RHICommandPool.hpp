/*
 * RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <cstdint>
#include "RHICommandBuffer.hpp"
#include "Status.hpp"

namespace Flux {

class RHICommandPool {
public:

    virtual Status allocateCommandBuffers(uint32_t bufferCount, RHICommandBuffer **buffers) = 0;

    virtual void freeCommandBuffers(uint32_t bufferCount, RHICommandBuffer *buffers) = 0;

    virtual void destroy(void) = 0;

};

}
