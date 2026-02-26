/*
 * Example application.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <fstream>
#include <iostream>
#include "Log.hpp"
#include "RHI.hpp"
#include "Window.hpp"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

using namespace Flux;

static constexpr const char *WINDOW_TITLE = "Flux Example Application";
static constexpr int WINDOW_WIDTH = 640;
static constexpr int WINDOW_HEIGHT = 480;
static constexpr RenderingAPI renderingApi = RenderingAPI::Vulkan;

static Window window;
static RHI *rhi = nullptr;
static RHIDevice *device = nullptr;
static RHIPipeline *pipeline = nullptr;
static RHICommandPool *commandPool = nullptr;

static void cleanup(void) {
    FLUX_LOG_INFO("Cleaning up...");

    if (commandPool != nullptr) {
        commandPool->destroy();
        commandPool = nullptr;
    }

    if (pipeline != nullptr) {
        pipeline->destroy();
        pipeline = nullptr;
    }

    if (device != nullptr) {
        device->destroy();
        device = nullptr;
    }

    if (rhi != nullptr) {
        rhi->shutdown();
        rhi = nullptr;
    }

    window.destroy();
    Window::globalCleanup();
}

static Status initialize(void) {
    Status status;

    const RHIShaderStageDescription shaderStageDescriptions[] = {
        {
            .type = RHIShaderStageType::vertex,
            .codeSize = 0,
            .code = nullptr,
            .entryPointName = "main",
        },
        {
            .type = RHIShaderStageType::fragment,
            .codeSize = 0,
            .code = nullptr,
            .entryPointName = "main",
        },
    };

    const RHIVertexInputAttributeDescription vertexInputAttributeDescriptions[] = {
        {
            .location = 0,
            .componentType = RHIComponentType::float32,
            .componentCount = 3,
            .offset = 0,
        },
    };

    const RHIVertexInputBindingDescription vertexInputBindingDescriptions[] = {
        {
            .stride = 3 * sizeof(float),
            .inputRate = RHIVertexInputRate::vertex,
            .attributeCount = ARRAY_SIZE(vertexInputAttributeDescriptions),
            .attributeDescriptions = vertexInputAttributeDescriptions,
        },
    };

    const RHIPipelineDescription pipelineDescription = {
        .primitiveTopology = RHIPrimitiveTopology::triangleList,

        .shaderStageCount = ARRAY_SIZE(shaderStageDescriptions),
        .shaderStageDescriptions = shaderStageDescriptions,

        .vertexInputBindingCount = ARRAY_SIZE(vertexInputBindingDescriptions),
        .vertexInputBindingDescriptions = vertexInputBindingDescriptions,
    };

    FLUX_LOG_INFO("Initializing...");

    status = window.create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, renderingApi);
    if (status != Status::success) {
        return status;
    }

    rhi = RHI::create(renderingApi);
    if (rhi == nullptr) {
        return status;
    }

    status = rhi->initialize();
    if (status != Status::success) {
        return status;
    }

    status = rhi->createDevice(window, &device);
    if (status != Status::success) {
        return status;
    }

    status = device->createPipeline(&pipelineDescription, &pipeline);
    if (status != Status::success) {
        return status;
    }

    status = device->createCommandPool(&commandPool);
    if (status != Status::success) {
        return status;
    }

    return Status::success;
}

int main(int argc, char **argv) {
    Status status;

    status = initialize();
    if (status != Status::success) {
        cleanup();
        return -1;
    }

    FLUX_LOG_INFO("Presenting window...");
    window.present();

    cleanup();
    return 0;
}
