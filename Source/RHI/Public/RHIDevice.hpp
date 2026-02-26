/*
 * RHI device.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "RHICommandPool.hpp"
#include "RHIPipeline.hpp"
#include "Status.hpp"

namespace Flux {

class RHIDevice {
public:

    virtual Status createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipeline) = 0;

    virtual Status createCommandPool(RHICommandPool **commandPool) = 0;

    virtual void destroy(void) = 0;

};

}
