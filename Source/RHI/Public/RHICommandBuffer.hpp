/*
 * RHI command buffer.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

namespace Flux {

class RHICommandBuffer {
public:

    virtual void free(void) = 0;

};

}
