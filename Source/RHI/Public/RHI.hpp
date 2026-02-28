/*
 * RHI (Rendering Hardware Interface).
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "RHIDevice.hpp"

namespace Flux {

class RHI {
public:

    static RHI *create(RenderingAPI renderingApi);

    virtual Status initialize(void) = 0;

    virtual Status createDevice(Window window, RHIDevice **device) = 0;

    virtual void shutdown(void) = 0;

};

}
