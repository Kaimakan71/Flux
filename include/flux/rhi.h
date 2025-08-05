/*
 * RHI (Rendering Hardware Interface).
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _FLUX_RHI_H
#define _FLUX_RHI_H

#include <flux/status.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct FluxRhiDevice_T *FluxRhiDevice;

typedef struct FluxRhi {
    void *data;

    FluxStatus (*createDevice)(struct FluxRhi *rhi, GLFWwindow *window, FluxRhiDevice *deviceOut);
    void (*destroyDevice)(struct FluxRhi *rhi, FluxRhiDevice device);

    void (*shutdown)(struct FluxRhi *rhi);
} FluxRhi;

typedef enum {
    FLUX_RHI_TYPE_NONE,
    FLUX_RHI_TYPE_VULKAN,
    FLUX_RHI_TYPE_OPENGL,
} FluxRhiType;

FluxStatus fluxRhiInitialize(FluxRhi *rhi, FluxRhiType type);

#endif /* !_FLUX_RHI_H */
