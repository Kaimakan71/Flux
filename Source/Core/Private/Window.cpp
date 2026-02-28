/*
 * Window management utilities.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "Window.hpp"

/* TODO: Make this detectable/configurable */
static constexpr int OPENGL_CONTEXT_VERSION_MAJOR = 3;
static constexpr int OPENGL_CONTEXT_VERSION_MINOR = 3;

static bool globalInitialized = false;

static void glfwErrorCallback(int code, const char *description) {
    FLUX_LOG_ERROR("GLFW error 0x%08x: %s", code, description);
}

namespace Flux {

void Window::globalCleanup(void) {
    if (globalInitialized) {
        glfwTerminate();
        globalInitialized = false;
    }
}

void Window::destroy(void) {
    if (this->handle != nullptr) {
        glfwDestroyWindow(this->handle);
        this->handle = nullptr;
    }
}

Status Window::create(const char *title, int width, int height, RenderingAPI renderingApi) {
    /* Initialize GLFW if needed */
    if (!globalInitialized) {
        FLUX_LOG_INFO("Initializing GLFW...");
        glfwSetErrorCallback(glfwErrorCallback);
        if (glfwInit() != GLFW_TRUE) {
            FLUX_LOG_ERROR("Failed to initialize GLFW");
            return Status::unknownError;
        }

        globalInitialized = true;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (renderingApi == RenderingAPI::OpenGL) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_CONTEXT_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_CONTEXT_VERSION_MINOR);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    FLUX_LOG_INFO("Creating window...");
    this->handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (this->handle == nullptr) {
        FLUX_LOG_ERROR("Failed to create window");
        return Status::unknownError;
    }

    if (renderingApi == RenderingAPI::OpenGL) {
        glfwMakeContextCurrent(this->handle);
        glfwSwapInterval(1);
    }

    return Status::success;
}

}
