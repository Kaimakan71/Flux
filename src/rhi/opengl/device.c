/*
 * OpenGL device.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/opengl.h"

#include <flux/log.h>
#include <stdlib.h>

void
rhiOpenglDestroyDevice(FluxRhi *rhi, RhiOpenglDevice *device)
{
    if (rhi == NULL || device == NULL) {
        return;
    }

    fluxLogDebug("destroying device...");

    if (device->window == glfwGetCurrentContext()) {
        glfwMakeContextCurrent(NULL);
    }

    free(device);
}

FluxStatus
rhiOpenglCreateDevice(FluxRhi *rhi, GLFWwindow *window, RhiOpenglDevice **deviceOut)
{
    RhiOpenglDevice *device;

    if (rhi == NULL || window == NULL || deviceOut == NULL) {
        return FLUX_STATUS_INVALID_ARGUMENT;
    }

    fluxLogDebug("creating device...");

    device = malloc(sizeof(*device));
    if (device == NULL) {
        return FLUX_STATUS_OUT_OF_HOST_MEMORY;
    }

    device->window = window;

    *deviceOut = device;
    return FLUX_STATUS_SUCCESS;
}
