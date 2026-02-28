/*
 * RHI rendering agent.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

namespace Flux {

class RHIRenderingAgent {
public:

    virtual void beginFrame(RHICommandBuffer *commandBuffer, RHIPipeline *pipeline) = 0;

    virtual void destroy(void) = 0;

};

}
