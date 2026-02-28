/*
 * Vulkan RHI rendering agent.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanLog.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanRenderingAgent.hpp"
#include "VulkanResult.hpp"

namespace Flux {

VulkanRenderingAgent::VulkanRenderingAgent(VulkanDevice &device): device(device) {
    this->commandPool = nullptr;
    for (uint32_t s = 0; s < MAX_FRAMES_IN_FLIGHT; s++) {
        this->commandBuffers[s] = VK_NULL_HANDLE;
        this->imageAvailable[s] = VK_NULL_HANDLE;
        this->inFlight[s] = VK_NULL_HANDLE;
        this->renderComplete[s] = VK_NULL_HANDLE;
    }
}

void VulkanRenderingAgent::destroy(void) {
    for (uint32_t f = 0; f < MAX_FRAMES_IN_FLIGHT; f++) {
        if (this->renderComplete[f] == VK_NULL_HANDLE) {
            break;
        }

        this->device.dispatch.vkDestroySemaphore(this->device.device, this->renderComplete[f], nullptr);
        this->renderComplete[f] = VK_NULL_HANDLE;
    }

    for (uint32_t f = 0; f < MAX_FRAMES_IN_FLIGHT; f++) {
        if (this->inFlight[f] == VK_NULL_HANDLE) {
            break;
        }

        this->device.dispatch.vkDestroyFence(this->device.device, this->inFlight[f], nullptr);
        this->inFlight[f] = VK_NULL_HANDLE;
    }

    for (uint32_t f = 0; f < MAX_FRAMES_IN_FLIGHT; f++) {
        if (this->imageAvailable[f] == VK_NULL_HANDLE) {
            break;
        }

        this->device.dispatch.vkDestroySemaphore(this->device.device, this->imageAvailable[f], nullptr);
        this->imageAvailable[f] = VK_NULL_HANDLE;
    }

    if (this->commandPool != nullptr) {
        this->commandPool->destroy();
        this->commandPool = nullptr;
    }
}

VkResult VulkanRenderingAgent::createSyncObjects(void) {
    VkResult result;

    static const VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    static const VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    FLUX_LOG_DEBUG("Creating synchronization objects...");

    for (uint32_t f = 0; f < MAX_FRAMES_IN_FLIGHT; f++) {
        result = this->device.dispatch.vkCreateSemaphore(this->device.device, &semaphoreCreateInfo, nullptr, &this->imageAvailable[f]);
        if (result != VK_SUCCESS) {
            FLUX_LOG_VULKAN_ERROR(result, "Failed to create image available semaphore");
            return result;
        }
    }

    for (uint32_t f = 0; f < MAX_FRAMES_IN_FLIGHT; f++) {
        result = this->device.dispatch.vkCreateFence(this->device.device, &fenceCreateInfo, nullptr, &this->inFlight[f]);
        if (result != VK_SUCCESS) {
            FLUX_LOG_VULKAN_ERROR(result, "Failed to create in-flight fence");
            return result;
        }
    }

    for (uint32_t f = 0; f < MAX_FRAMES_IN_FLIGHT; f++) {
        result = this->device.dispatch.vkCreateSemaphore(this->device.device, &semaphoreCreateInfo, nullptr, &this->renderComplete[f]);
        if (result != VK_SUCCESS) {
            FLUX_LOG_VULKAN_ERROR(result, "Failed to create render complete semaphore");
            return result;
        }
    }

    return VK_SUCCESS;
}

Status VulkanRenderingAgent::create(void) {
    Status status;
    VkResult result;

    static VkCommandBufferAllocateInfo allocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = VK_NULL_HANDLE,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = UINT32_MAX,
    };

    status = this->device.createCommandPool(reinterpret_cast<RHICommandPool **>(&this->commandPool));
    if (status != Status::success) {
        return status;
    }

    FLUX_LOG_DEBUG("Allocating command buffers...");
    allocateInfo.commandPool = this->commandPool->pool;
    allocateInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;
    result = this->device.dispatch.vkAllocateCommandBuffers(this->device.device, &allocateInfo, this->commandBuffers);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to allocate %u command buffer(s)", MAX_FRAMES_IN_FLIGHT);
        return VulkanResult::getStatus(result);
    }

    result = this->createSyncObjects();
    if (result != VK_SUCCESS) {
        this->destroy();
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

void VulkanRenderingAgent::drawFrame(uint32_t currentFrame, VkPipeline pipeline) {
    uint32_t imageIndex;

    static const VkCommandBufferBeginInfo commandBufferBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };

    static const VkClearValue clearValue = {
        .color = {
            .float32 = { 0.0f, 0.0f, 0.0f, 1.0f },
        },
    };

    static VkRenderPassBeginInfo renderPassBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = VK_NULL_HANDLE,
        .framebuffer = VK_NULL_HANDLE,
        .renderArea = {
            .offset = {
                .x = 0,
                .y = 0,
            },
            .extent = {
                .width = 0,
                .height = 0,
            }
        },
        .clearValueCount = 1,
        .pClearValues = &clearValue,
    };

    static const VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) this->device.swapchainImageExtent.width,
        .height = (float) this->device.swapchainImageExtent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    static const VkRect2D scissor = {
        .offset = {
            .x = 0,
            .y = 0,
        },
        .extent = this->device.swapchainImageExtent,
    };

    static const VkPipelineStageFlags waitStages[] = { 
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    };

    static VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &this->imageAvailable[currentFrame],
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &this->commandBuffers[currentFrame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &this->renderComplete[currentFrame],
    };

    static VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &this->renderComplete[currentFrame],
        .swapchainCount = 1,
        .pSwapchains = &this->device.swapchain,
        .pImageIndices = &imageIndex,
        .pResults = nullptr,
    };

    /* Wait for in-flight frame to finish rendering */
    this->device.dispatch.vkWaitForFences(this->device.device, 1, &this->inFlight[currentFrame], VK_TRUE, UINT64_MAX);
    this->device.dispatch.vkResetFences(this->device.device, 1, &this->inFlight[currentFrame]);

    /* Acquire an image to render to */
    this->device.dispatch.vkAcquireNextImageKHR(this->device.device, this->device.swapchain, UINT64_MAX, this->imageAvailable[currentFrame], VK_NULL_HANDLE, &imageIndex);

    this->device.dispatch.vkResetCommandBuffer(this->commandBuffers[currentFrame], 0);
    this->device.dispatch.vkBeginCommandBuffer(this->commandBuffers[currentFrame], &commandBufferBeginInfo);

    renderPassBeginInfo.renderPass = this->device.renderPass;
    renderPassBeginInfo.renderArea.extent = this->device.swapchainImageExtent;
    renderPassBeginInfo.framebuffer = this->device.swapchainFramebuffers[imageIndex];
    this->device.dispatch.vkCmdBeginRenderPass(this->commandBuffers[currentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    this->device.dispatch.vkCmdBindPipeline(this->commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    this->device.dispatch.vkCmdSetViewport(this->commandBuffers[currentFrame], 0, 1, &viewport);
    this->device.dispatch.vkCmdSetScissor(this->commandBuffers[currentFrame], 0, 1, &scissor);

    this->device.dispatch.vkCmdDraw(this->commandBuffers[currentFrame], 3, 1, 0, 0);

    this->device.dispatch.vkCmdEndRenderPass(this->commandBuffers[currentFrame]);
    this->device.dispatch.vkEndCommandBuffer(this->commandBuffers[currentFrame]);

    /* Submit rendering commands as soon as an image is available */
    this->device.dispatch.vkQueueSubmit(this->device.info.graphicsQueue, 1, &submitInfo, this->inFlight[currentFrame]);

    /* Present the rendered image as soon as rendering is complete */
    this->device.dispatch.vkQueuePresentKHR(this->device.info.presentQueue, &presentInfo);
}

void VulkanRenderingAgent::present(RHIPipeline &pipeline) {
    uint32_t currentFrame;
    VkPipeline vkPipeline;

    currentFrame = 0;
    vkPipeline = (reinterpret_cast<VulkanPipeline &>(pipeline)).pipeline;

    glfwShowWindow(this->device.window.handle);
    while (!glfwWindowShouldClose(this->device.window.handle)) {
        glfwPollEvents();
        this->drawFrame(currentFrame, vkPipeline);
    }
    this->device.dispatch.vkDeviceWaitIdle(this->device.device);
    glfwHideWindow(this->device.window.handle);
}

}
