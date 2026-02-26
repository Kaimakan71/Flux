/*
 * OpenGL RHI device.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "OpenGLDispatch.hpp"
#include "OpenGLRHI.hpp"

namespace Flux {

class OpenGLDevice: public RHIDevice {
private:

    GLFWwindow *context;
    OpenGLDispatch dispatch;

    void bind(void);

public:

    Status create(OpenGLRHI *rhi, Window window);

    virtual Status createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipeline);

    virtual Status createCommandPool(RHICommandPool **commandPool);

    virtual void destroy(void);

};

}
