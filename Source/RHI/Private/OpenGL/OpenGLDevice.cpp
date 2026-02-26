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

namespace Flux {

void OpenGLDevice::destroy(void) {

}

void OpenGLDevice::bind(void) {
    if (glfwGetCurrentContext() != this->context) {
        glfwMakeContextCurrent(this->context);
    }
}

Status OpenGLDevice::create(OpenGLRHI *rhi, Window window) {
    Status status;

    this->context = window.handle;
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

    pipeline = new(std::nothrow) OpenGLPipeline();
    if (pipeline == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = pipeline->create(this, description);
    if (status != Status::success) {
        delete pipeline;
        return status;
    }

    *pipelineOut = pipeline;
    return Status::success;
}

Status OpenGLDevice::createCommandPool(RHICommandPool **commandPoolOut) {
    Status status;
    OpenGLCommandPool *commandPool;

    commandPool = new(std::nothrow) OpenGLCommandPool();
    if (commandPool == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = commandPool->create(this);
    if (status != Status::success) {
        delete commandPool;
        return status;
    }

    *commandPoolOut = commandPool;
    return Status::success;
}

}
