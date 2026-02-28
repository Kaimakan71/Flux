/*
 * OpenGL RHI device.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <new>
#include "Log.hpp"
#include "OpenGLCommandPool.hpp"
#include "OpenGLLoader.hpp"
#include "OpenGLPipeline.hpp"
#include "OpenGLRenderingAgent.hpp"

namespace Flux {

OpenGLDevice::OpenGLDevice(Window &window) {
    this->context = window.handle;
}

void OpenGLDevice::destroy(void) {

}

Status OpenGLDevice::create(void) {
    Status status;

    this->bind();

    status = OpenGLLoader::loadSymbols(&this->dispatch, glfwGetProcAddress);
    if (status != Status::success) {
        return status;
    }

    FLUX_LOG_DEBUG("Vendor: \"%s\"", dispatch.glGetString(GL_VENDOR));
    FLUX_LOG_DEBUG("Renderer: \"%s\"", dispatch.glGetString(GL_RENDERER));
    FLUX_LOG_DEBUG("Version: \"%s\"", dispatch.glGetString(GL_VERSION));

    return Status::success;
}

Status OpenGLDevice::createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipelineOut) {
    Status status;
    OpenGLPipeline *pipeline;

    pipeline = new(std::nothrow) OpenGLPipeline(*this);
    if (pipeline == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = pipeline->create(description);
    if (status != Status::success) {
        delete pipeline;
        return status;
    }

    *pipelineOut = pipeline;
    return Status::success;
}

Status OpenGLDevice::createCommandPool(RHICommandPool **poolOut) {
    Status status;
    OpenGLCommandPool *pool;

    pool = new(std::nothrow) OpenGLCommandPool(*this);
    if (pool == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = pool->create();
    if (status != Status::success) {
        delete pool;
        return status;
    }

    *poolOut = pool;
    return Status::success;
}

Status OpenGLDevice::createRenderingAgent(RHIRenderingAgent **agentOut) {
    Status status;
    OpenGLRenderingAgent *agent;

    agent = new(std::nothrow) OpenGLRenderingAgent(*this);
    if (agent == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = agent->create();
    if (status != Status::success) {
        delete agent;
        return status;
    }

    *agentOut = agent;
    return Status::success;
}

}
