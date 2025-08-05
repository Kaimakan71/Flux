/*
 * OpenGL RHI.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _RHI_OPENGL_H
#define _RHI_OPENGL_H

#include "internal/rhi/glUtils.h"

#include <flux/rhi.h>

typedef struct FluxRhiDevice_T {
    GLFWwindow *window;
} RhiOpenglDevice;

FluxStatus rhiOpenglCreateDevice(FluxRhi *rhi, GLFWwindow *window, RhiOpenglDevice **deviceOut);
void rhiOpenglDestroyDevice(FluxRhi *rhi, RhiOpenglDevice *device);

#endif /* !_RHI_OPENGL_H */
