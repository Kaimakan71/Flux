/*
 * Flux example.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <flux/rhi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "Flux Engine Test"

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 0

#define DEFAULT_RHI_TYPE FLUX_RHI_TYPE_VULKAN

static void
glfwError(int code, const char *description)
{
    fprintf(stderr, "GLFW error 0x%08x: %s\n", code, description);
}

static GLFWwindow *
createWindow(FluxRhiType rhiType)
{
    GLFWwindow *window;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (rhiType == FLUX_RHI_TYPE_OPENGL) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        return NULL;
    }

    if (rhiType == FLUX_RHI_TYPE_OPENGL) {
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
    }

    return window;
}

int
main(int argc, char **argv)
{
    FluxRhiType rhiType;
    GLFWwindow *window;
    FluxStatus status;
    FluxRhi rhi;
    FluxRhiDevice device;

    if (argc >= 2) {
        if (strcmp(argv[1], "vulkan") == 0) {
            rhiType = FLUX_RHI_TYPE_VULKAN;
        } else if (strcmp(argv[1], "opengl") == 0) {
            rhiType = FLUX_RHI_TYPE_OPENGL;
        } else {
            fprintf(stderr, "invalid RHI type \"%s\"\n", argv[1]);
            return EXIT_FAILURE;
        }
    } else {
        rhiType = DEFAULT_RHI_TYPE;
    }

    glfwSetErrorCallback(glfwError);
    if (!glfwInit()) {
        fprintf(stderr, "failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    window = createWindow(rhiType);
    if (window == NULL) {
        fprintf(stderr, "failed to create window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    status = fluxRhiInitialize(&rhi, rhiType);
    if (status != FLUX_STATUS_SUCCESS) {
        fprintf(stderr, "failed to initialize RHI\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    status = rhi.createDevice(&rhi, window, &device);
    if (status != FLUX_STATUS_SUCCESS) {
        fprintf(stderr, "failed to create device\n");
        rhi.shutdown(&rhi);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwShowWindow(window);
    while (!glfwWindowShouldClose(window)) {
        /* glfwSwapBuffers(window); */
        glfwPollEvents();
    }
    glfwHideWindow(window);

    rhi.destroyDevice(&rhi, device);
    rhi.shutdown(&rhi);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
