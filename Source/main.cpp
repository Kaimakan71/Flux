/*
 * Example application.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <filesystem>
#include <fstream>
#include <vector>
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

static std::vector<uint8_t> loadFile(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    std::streamsize size = file.tellg();
    if (size < 0) {
        throw std::runtime_error("Failed to get file size");
    }

    std::vector<uint8_t> buffer(static_cast<size_t>(size));

    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file");
    }

    file.close();
    return buffer;
}

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

    std::vector<uint8_t> vertCode = loadFile("vert.spv");
    std::vector<uint8_t> fragCode = loadFile("frag.spv");

    const RHIShaderStageDescription shaderStageDescriptions[] = {
        {
            .type = RHIShaderStageType::vertex,
            .codeSize = vertCode.size(),
            .code = vertCode.data(),
            .entryPointName = "main",
        },
        {
            .type = RHIShaderStageType::fragment,
            .codeSize = fragCode.size(),
            .code = fragCode.data(),
            .entryPointName = "main",
        },
    };

    const RHIPipelineDescription pipelineDescription = {
        .primitiveTopology = RHIPrimitiveTopology::triangleList,

        .shaderStageCount = ARRAY_SIZE(shaderStageDescriptions),
        .shaderStageDescriptions = shaderStageDescriptions,

        .vertexInputBindingCount = 0,
        .vertexInputBindingDescriptions = nullptr,
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
