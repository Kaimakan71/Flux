/*
 * OpenGL RHI rendering agent.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "OpenGLRenderingAgent.hpp"

namespace Flux {

OpenGLRenderingAgent::OpenGLRenderingAgent(OpenGLDevice &device): device(device) {

}

void OpenGLRenderingAgent::destroy(void) {

}

Status OpenGLRenderingAgent::create(void) {
    FLUX_LOG_ERROR("OpenGL rendering agent creation not implemented");
    return Status::notImplemented;
}

void OpenGLRenderingAgent::present(RHIPipeline &pipeline) {

}

}
