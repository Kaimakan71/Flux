/*
 * OpenGL RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "OpenGLCommandPool.hpp"

namespace Flux {

void OpenGLCommandPool::destroy(void) {

}

Status OpenGLCommandPool::create(OpenGLDevice *device) {
    FLUX_LOG_ERROR("OpenGL command pool not implemented");
    return Status::notImplemented;
}

}
