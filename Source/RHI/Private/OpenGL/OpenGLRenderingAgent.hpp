/*
 * OpenGL RHI rendering agent.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "OpenGLDevice.hpp"

namespace Flux {

class OpenGLRenderingAgent: public RHIRenderingAgent {
public:

    Status create(OpenGLDevice *device);

    virtual void present(Window window, RHICommandBuffer *commandBuffer, RHIPipeline *pipeline);

    virtual void destroy(void);

};

}
