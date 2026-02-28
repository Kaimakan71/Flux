/*
 * OpenGL RHI.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "OpenGLDevice.hpp"

namespace Flux {

void OpenGLRHI::shutdown(void) {
    FLUX_LOG_DEBUG("Shutting down OpenGL RHI...");
}

Status OpenGLRHI::initialize(void) {
    FLUX_LOG_DEBUG("Initializing OpenGL RHI...");
    return Status::success;
}

Status OpenGLRHI::createDevice(Window &window, RHIDevice **deviceOut) {
    OpenGLDevice *device;
    Status status;

    device = new(std::nothrow) OpenGLDevice(window);
    if (device == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = device->create();
    if (status != Status::success) {
        delete device;
        return status;
    }

    *deviceOut = device;
    return Status::success;
}

}
