/*
 * OpenGL RHI pipeline.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "OpenGLPipeline.hpp"

namespace Flux {

OpenGLPipeline::OpenGLPipeline(OpenGLDevice &device): device(device) {

}

void OpenGLPipeline::destroy(void) {

}

Status OpenGLPipeline::create(const RHIPipelineDescription *description) {
    FLUX_LOG_ERROR("OpenGL pipeline not implemented");
    return Status::notImplemented;
}

}
