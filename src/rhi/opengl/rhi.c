/*
 * OpenGL RHI.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/opengl.h"

#include <flux/log.h>
#include <stdlib.h>

static GLboolean libraryLoaded = GL_FALSE;

static GLboolean
loadLibrary(void)
{
    GlUtilsLibraryVersion version;

    if (libraryLoaded) {
        return GL_TRUE;
    }

    fluxLogDebug("loading symbols...");
    if (!glUtilsLoadSymbols((GlUtilsGetSymbolAddress)glfwGetProcAddress, &version)) {
        return GL_FALSE;
    }

    fluxLogDebug("loaded version: %d.%d", version.major, version.minor);

    libraryLoaded = GL_TRUE;
    return GL_TRUE;
}

void
rhiOpenglShutdown(FluxRhi *rhi)
{
    if (rhi == NULL) {
        return;
    }

    fluxLogDebug("shutting down RHI...");
}

FluxStatus
rhiOpenglInitialize(FluxRhi *rhi)
{
    if (rhi == NULL) {
        return FLUX_STATUS_INVALID_ARGUMENT;
    }

    fluxLogDebug("initializing RHI...");

    if (!loadLibrary()) {
        return FLUX_STATUS_INCOMPATIBLE_DRIVER;
    }

    rhi->data = NULL;
    rhi->createDevice = rhiOpenglCreateDevice;
    rhi->destroyDevice = rhiOpenglDestroyDevice;
    rhi->shutdown = rhiOpenglShutdown;

    return FLUX_STATUS_SUCCESS;
}
