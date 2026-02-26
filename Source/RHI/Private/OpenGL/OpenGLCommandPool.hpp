/*
 * OpenGL RHI command pool.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "OpenGLDevice.hpp"

namespace Flux {

class OpenGLCommandPool: public RHICommandPool {
public:

    Status create(OpenGLDevice *device);

    virtual void destroy(void);

};

}
