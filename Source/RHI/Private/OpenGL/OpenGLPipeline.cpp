/*
 * OpenGL RHI pipeline.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "OpenGLPipeline.hpp"

namespace Flux {

void OpenGLPipeline::destroy(void) {

}

Status OpenGLPipeline::create(OpenGLDevice *device, const RHIPipelineDescription *description) {
    FLUX_LOG_ERROR("OpenGL pipeline not implemented");
    return Status::notImplemented;
}

}
