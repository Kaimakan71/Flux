/*
 * RHI (Rendering Hardware Interface).
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <new>
#include "OpenGL/OpenGLRHI.hpp"
#include "Vulkan/VulkanRHI.hpp"

namespace Flux {

RHI *RHI::create(RenderingAPI renderingApi) {
    switch (renderingApi) {
    case RenderingAPI::OpenGL:
        return new(std::nothrow) OpenGLRHI();
    case RenderingAPI::Vulkan:
        return new(std::nothrow) VulkanRHI();
    default:
        return nullptr;
    }
}

}
