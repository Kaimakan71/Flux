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

    void bind(void) {
        if (glfwGetCurrentContext() != this->context) {
            glfwMakeContextCurrent(this->context);
        }
    }

public:

    GLFWwindow *context;
    OpenGLDispatch dispatch;

    OpenGLDevice(Window &window);

    Status create(void);

    virtual Status createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipeline);

    virtual Status createCommandPool(RHICommandPool **pool);

    virtual Status createRenderingAgent(RHIRenderingAgent **agent);

    virtual void destroy(void);

};

}
