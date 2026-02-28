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
    this->imageAvailable = VK_NULL_HANDLE;
    this->inFlight = VK_NULL_HANDLE;
    this->renderComplete = VK_NULL_HANDLE;
}

void VulkanRenderingAgent::destroy(void) {
    if (this->renderComplete != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying render complete semaphore...");
        this->device.dispatch.vkDestroySemaphore(this->device.device, this->renderComplete, nullptr);
        this->renderComplete = VK_NULL_HANDLE;
    }

    if (this->inFlight != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying in-flight fence...");
        this->device.dispatch.vkDestroyFence(this->device.device, this->inFlight, nullptr);
        this->inFlight = VK_NULL_HANDLE;
    }

    if (this->imageAvailable != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying image available semaphore...");
        this->device.dispatch.vkDestroySemaphore(this->device.device, this->imageAvailable, nullptr);
        this->imageAvailable = VK_NULL_HANDLE;
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

    result = this->device.dispatch.vkCreateSemaphore(this->device.device, &semaphoreCreateInfo, nullptr, &this->imageAvailable);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create image available semaphore");
        return result;
    }

    result = this->device.dispatch.vkCreateFence(this->device.device, &fenceCreateInfo, nullptr, &this->inFlight);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create in-flight fence");
        return result;
    }

    result = this->device.dispatch.vkCreateSemaphore(this->device.device, &semaphoreCreateInfo, nullptr, &this->renderComplete);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create render complete semaphore");
        return result;
    }

    return VK_SUCCESS;
}

Status VulkanRenderingAgent::create(void) {
    VkResult result;

    result = this->createSyncObjects();
    if (result != VK_SUCCESS) {
        this->destroy();
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

void VulkanRenderingAgent::drawFrame(VkCommandBuffer commandBuffer, VkPipeline pipeline) {
    uint32_t imageIndex;

    static const VkCommandBufferBeginInfo commandBufferBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };

    static const VkClearValue clearValue = {
        .color = {
            .float32 = { 0.015f, 0.015f, 0.050f, 1.0f },
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
        .pWaitSemaphores = &this->imageAvailable,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &this->renderComplete,
    };

    static VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &this->renderComplete,
        .swapchainCount = 1,
        .pSwapchains = &this->device.swapchain,
        .pImageIndices = &imageIndex,
        .pResults = nullptr,
    };

    /* Wait for in-flight frame to finish rendering */
    this->device.dispatch.vkWaitForFences(this->device.device, 1, &this->inFlight, VK_TRUE, UINT64_MAX);
    this->device.dispatch.vkResetFences(this->device.device, 1, &this->inFlight);

    /* Acquire an image to render to */
    this->device.dispatch.vkAcquireNextImageKHR(this->device.device, this->device.swapchain, UINT64_MAX, this->imageAvailable, VK_NULL_HANDLE, &imageIndex);

    this->device.dispatch.vkResetCommandBuffer(commandBuffer, 0);
    this->device.dispatch.vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

    renderPassBeginInfo.renderPass = this->device.renderPass;
    renderPassBeginInfo.renderArea.extent = this->device.swapchainImageExtent;
    renderPassBeginInfo.framebuffer = this->device.swapchainFramebuffers[imageIndex];
    this->device.dispatch.vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    this->device.dispatch.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    this->device.dispatch.vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    this->device.dispatch.vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    this->device.dispatch.vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    this->device.dispatch.vkCmdEndRenderPass(commandBuffer);
    this->device.dispatch.vkEndCommandBuffer(commandBuffer);

    /* Submit rendering commands as soon as an image is available */
    this->device.dispatch.vkQueueSubmit(this->device.info.graphicsQueue, 1, &submitInfo, this->inFlight);

    /* Present the rendered image as soon as rendering is complete */
    this->device.dispatch.vkQueuePresentKHR(this->device.info.presentQueue, &presentInfo);
}

void VulkanRenderingAgent::present(RHICommandBuffer &commandBuffer, RHIPipeline &pipeline) {
    VkCommandBuffer vkCommandBuffer;
    VkPipeline vkPipeline;

    vkCommandBuffer = (reinterpret_cast<VulkanCommandBuffer &>(commandBuffer)).buffer;
    vkPipeline = (reinterpret_cast<VulkanPipeline &>(pipeline)).pipeline;

    glfwShowWindow(this->device.window.handle);
    while (!glfwWindowShouldClose(this->device.window.handle)) {
        glfwPollEvents();
        this->drawFrame(vkCommandBuffer, vkPipeline);
    }
    this->device.dispatch.vkDeviceWaitIdle(this->device.device);
    glfwHideWindow(this->device.window.handle);
}

}
