/*
 * Window management utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "RenderingAPI.hpp"
#include "Status.hpp"

namespace Flux {

class Window {
public:

    GLFWwindow *handle;

    static void globalCleanup(void);

    Status create(const char *title, int width, int height, RenderingAPI renderingApi);

    void destroy(void);

};

}
