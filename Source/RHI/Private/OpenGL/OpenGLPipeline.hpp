/*
 * OpenGL RHI pipeline.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "OpenGLDevice.hpp"

namespace Flux {

class OpenGLPipeline: public RHIPipeline {
public:

    Status create(OpenGLDevice *device, const RHIPipelineDescription *description);

    virtual void destroy(void);

};

}
