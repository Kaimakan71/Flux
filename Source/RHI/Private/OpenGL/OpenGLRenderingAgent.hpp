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

    OpenGLDevice &device;

    OpenGLRenderingAgent(OpenGLDevice &device);

    Status create(void);

    virtual void present(RHIPipeline &pipeline);

    virtual void destroy(void);

};

}
