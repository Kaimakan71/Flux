/*
 * OpenGL RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "OpenGLCommandPool.hpp"

namespace Flux {

OpenGLCommandPool::OpenGLCommandPool(OpenGLDevice &device): device(device) {

}

void OpenGLCommandPool::destroy(void) {

}

Status OpenGLCommandPool::create(void) {
    FLUX_LOG_ERROR("OpenGL command pool not implemented");
    return Status::notImplemented;
}

Status OpenGLCommandPool::allocateCommandBuffers(uint32_t bufferCount, RHICommandBuffer **buffersOut) {
    FLUX_LOG_ERROR("OpenGL command buffer allocation not implemented");
    return Status::notImplemented;
}

void OpenGLCommandPool::freeCommandBuffers(uint32_t bufferCount, RHICommandBuffer *buffers) {

}

}
