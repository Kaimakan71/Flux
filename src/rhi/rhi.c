/*
 * RHI (Rendering Hardware Interface).
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <flux/rhi.h>
#include <stddef.h>

FluxStatus rhiVulkanInitialize(FluxRhi *rhi);
FluxStatus rhiOpenglInitialize(FluxRhi *rhi);

FluxStatus
fluxRhiInitialize(FluxRhi *rhi, FluxRhiType type)
{
    if (rhi == NULL) {
        return FLUX_STATUS_INVALID_ARGUMENT;
    }

    switch (type) {
    case FLUX_RHI_TYPE_VULKAN:
        return rhiVulkanInitialize(rhi);
    case FLUX_RHI_TYPE_OPENGL:
        return rhiOpenglInitialize(rhi);
    default:
        return FLUX_STATUS_INVALID_ARGUMENT;
    }
}
