/*
 * OpenGL RHI.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "RHI.hpp"

namespace Flux {

class OpenGLRHI: public RHI {
public:

    virtual Status initialize(void);

    virtual Status createDevice(Window &window, RHIDevice **device);

    virtual void shutdown(void);

};

}
