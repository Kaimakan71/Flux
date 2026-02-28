/*
 * Vulkan RHI device.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <algorithm>
#include <cstring>
#include "Log.hpp"
#include "VulkanCommandPool.hpp"
#include "VulkanLoader.hpp"
#include "VulkanLog.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanRenderingAgent.hpp"
#include "VulkanResult.hpp"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

static const char *requiredExtensionNames[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

namespace Flux {

const char *VulkanDevice::getPhysicalDeviceTypeString(VkPhysicalDeviceType type) {
    switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        return "other";
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        return "integrated GPU";
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        return "discrete GPU";
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        return "virtual GPU";
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        return "CPU";
    default:
        return "unknown";
    }
}

const char *VulkanDevice::getPresentModeString(VkPresentModeKHR mode) {
    switch (mode) {
    case VK_PRESENT_MODE_IMMEDIATE_KHR:
        return "immediate";
    case VK_PRESENT_MODE_MAILBOX_KHR:
        return "mailbox";
    case VK_PRESENT_MODE_FIFO_KHR:
        return "FIFO";
    case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
        return "FIFO (relaxed)";
    case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
        return "shared demand refresh";
    case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
        return "shared continuous refresh";
    case VK_PRESENT_MODE_FIFO_LATEST_READY_KHR:
        return "FIFO (latest ready)";
    default:
        return "unknown";
    }
}

VkBool32 VulkanDevice::deviceIsCompatible(VkPhysicalDevice device, VulkanDeviceInfo *info) {
    VkResult result;

    if (!this->rhi->deviceSupportsExtensions(device, nullptr, ARRAY_SIZE(requiredExtensionNames), requiredExtensionNames)) {
        return VK_FALSE;
    }

    result = this->rhi->getDeviceInfo(device, this->surface, info);
    if (result != VK_SUCCESS) {
        return VK_FALSE;
    }

    if (info->surface.formatCount < 1 || info->surface.presentModeCount < 1) {
        FLUX_LOG_DEBUG("Physical device does not support at least one surface format and present mode, skipping");
        return VK_FALSE;
    }

    return VK_TRUE;
}

void VulkanDevice::destroy(void) {
    if (this->swapchainFramebuffers != nullptr) {
        FLUX_LOG_DEBUG("Destroying swapchain framebuffers...");

        for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
            this->dispatch.vkDestroyFramebuffer(this->device, this->swapchainFramebuffers[i], nullptr);
        }

        delete[] this->swapchainFramebuffers;
        this->swapchainFramebuffers = nullptr;
    }

    if (this->swapchainImageViews != nullptr) {
        FLUX_LOG_DEBUG("Destroying swapchain image views...");

        for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
            this->dispatch.vkDestroyImageView(this->device, this->swapchainImageViews[i], nullptr);
        }

        delete[] this->swapchainImageViews;
        this->swapchainImageViews = nullptr;
    }

    if (this->renderPass != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying render pass...");
        this->dispatch.vkDestroyRenderPass(this->device, this->renderPass, nullptr);
        this->renderPass = VK_NULL_HANDLE;
    }

    if (this->swapchain != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying swapchain...");
        this->dispatch.vkDestroySwapchainKHR(this->device, this->swapchain, nullptr);
        this->swapchain = VK_NULL_HANDLE;
    }

    if (this->device != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying logical device...");
        this->dispatch.vkDestroyDevice(this->device, nullptr);
        this->device = VK_NULL_HANDLE;
    }

    if (this->deviceInfo.surface.presentModes != nullptr) {
        delete[] this->deviceInfo.surface.presentModes;
        this->deviceInfo.surface.presentModes = nullptr;
        this->deviceInfo.surface.presentModeCount = 0;
    }

    if (this->deviceInfo.surface.formats != nullptr) {
        delete[] this->deviceInfo.surface.formats;
        this->deviceInfo.surface.formats = nullptr;
        this->deviceInfo.surface.formatCount = 0;
    }

    if (this->surface != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying window surface...");
        this->rhi->dispatch.vkDestroySurfaceKHR(this->rhi->instance, this->surface, nullptr);
        this->surface = VK_NULL_HANDLE;
    }
}

VkResult VulkanDevice::selectPhysicalDevice(void) {
    VkResult result;
    uint32_t deviceCount;
    VkPhysicalDevice *devices;

    FLUX_LOG_DEBUG("Selecting physical device...");

    result = this->rhi->getPhysicalDevices(&deviceCount, &devices);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to get physical device list");
        return result;
    }

    /* TODO: Use a point-based ranking system to get the best device */
    for (uint32_t d = 0; d < deviceCount; d++) {
        if (this->deviceIsCompatible(devices[d], &this->deviceInfo)) {
            this->physicalDevice = devices[d];
            delete[] devices;
            return VK_SUCCESS;
        }
    }

    FLUX_LOG_ERROR("Failed to find a suitable physical device");
    delete[] devices;
    return VK_ERROR_FEATURE_NOT_PRESENT;
}

void VulkanDevice::selectSwapchainParameters(Window window) {
    uint32_t imageCount;
    VkSurfaceCapabilitiesKHR *capabilities;
    VkSurfaceFormatKHR *formats;
    int width, height;

    FLUX_LOG_DEBUG("Selecting swapchain parameters...");

    /* Select image count */
    capabilities = &this->deviceInfo.surface.capabilities;
    imageCount = capabilities->minImageCount + 1;
    if (capabilities->maxImageCount != 0 &&  capabilities->maxImageCount < imageCount) {
        imageCount = capabilities->maxImageCount;
    }
    this->swapchainImageCount = imageCount;
    FLUX_LOG_DEBUG("Image count: %u", this->swapchainImageCount);

    /* Select image format */
    formats = this->deviceInfo.surface.formats;
    this->swapchainImageFormat = &formats[0];
    for (uint32_t f = 0; f < this->deviceInfo.surface.formatCount; f++) {
        if (formats[f].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            this->swapchainImageFormat = &formats[f];
            break;
        }
    }
    FLUX_LOG_DEBUG("Image format: format=%d, colorSpace=%d", this->swapchainImageFormat->format, this->swapchainImageFormat->colorSpace);

    /* Select image extent */
    if (capabilities->currentExtent.width != UINT32_MAX && capabilities->currentExtent.height != UINT32_MAX) {
        this->swapchainImageExtent = capabilities->currentExtent;
    } else {
        /* Get size in pixels to adjust for DPI */
        glfwGetFramebufferSize(window.handle, &width, &height);

        /* Use best possible valid size */
        this->swapchainImageExtent.width = std::clamp((uint32_t) width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
        this->swapchainImageExtent.height = std::clamp((uint32_t) height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);
    }
    FLUX_LOG_DEBUG("Image extent: %ux%u", this->swapchainImageExtent.width, this->swapchainImageExtent.height);

    /* Select present mode (mailbox is preferred) */
    this->swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (uint32_t m = 0; m < this->deviceInfo.surface.presentModeCount; m++) {
        if (this->deviceInfo.surface.presentModes[m] == VK_PRESENT_MODE_MAILBOX_KHR) {
            this->swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }
    FLUX_LOG_DEBUG("Present mode: %s (0x%08x)", getPresentModeString(this->swapchainPresentMode), (uint32_t) this->swapchainPresentMode);
}

VkResult VulkanDevice::getSwapchainImages(uint32_t *imageCountOut, VkImage **imagesOut) {
    VkResult result;
    uint32_t imageCount;
    VkImage *images;

    result = this->dispatch.vkGetSwapchainImagesKHR(this->device, this->swapchain, &imageCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (imageCount < 1) {
        *imageCountOut = 0;
        *imagesOut = nullptr;
        return VK_SUCCESS;
    }

    images = new(std::nothrow) VkImage[imageCount];
    if (images == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    result = this->dispatch.vkGetSwapchainImagesKHR(this->device, this->swapchain, &imageCount, images);
    if (result != VK_SUCCESS) {
        delete[] images;
        return result;
    }

    *imageCountOut = imageCount;
    *imagesOut = images;
    return VK_SUCCESS;
}

VkResult VulkanDevice::createSurface(Window window) {
    VkResult result;

    FLUX_LOG_DEBUG("Creating window surface...");
    result = glfwCreateWindowSurface(this->rhi->instance, window.handle, nullptr, &this->surface);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create window surface");
        return result;
    }

    return VK_SUCCESS;
}

VkResult VulkanDevice::createDevice(void) {
    VkResult result;
    float queuePriority;
    VkDeviceQueueCreateInfo queueInfos[2], *queueInfo;
    VkPhysicalDeviceFeatures features;

    static VkDeviceCreateInfo createInfo = {
        /* NOTE: enabledLayerCount and ppEnabledLayerNames are deprecated */
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 0,
        .pQueueCreateInfos = nullptr,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = ARRAY_SIZE(requiredExtensionNames),
        .ppEnabledExtensionNames = requiredExtensionNames,
        .pEnabledFeatures = nullptr,
    };

    result = this->selectPhysicalDevice();
    if (result != VK_SUCCESS) {
        return result;
    }

    FLUX_LOG_DEBUG("Type: %s (0x%08x)", getPhysicalDeviceTypeString(this->deviceInfo.properties.deviceType), (uint32_t) this->deviceInfo.properties.deviceType);
    FLUX_LOG_DEBUG("Vendor ID: 0x%08x", this->deviceInfo.properties.vendorID);
    FLUX_LOG_DEBUG("Device ID: 0x%08x", this->deviceInfo.properties.deviceID);
    FLUX_LOG_DEBUG("Name: \"%s\"", this->deviceInfo.properties.deviceName);
    FLUX_LOG_DEBUG("API version: %u.%u.%u", VK_API_VERSION_MAJOR(this->deviceInfo.properties.apiVersion), VK_API_VERSION_MINOR(this->deviceInfo.properties.apiVersion), VK_API_VERSION_PATCH(this->deviceInfo.properties.apiVersion));
    FLUX_LOG_DEBUG("Queue family indices: graphics=%u, present=%u", this->deviceInfo.queueFamilies.graphicsIndex, this->deviceInfo.queueFamilies.presentIndex);

    /* Create one queue for each queue family */
    queuePriority = 1.0f;
    for (uint32_t f = 0; f < this->deviceInfo.queueFamilies.count; f++) {
        queueInfo = &queueInfos[f];
        queueInfo->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo->pNext = nullptr;
        queueInfo->flags = 0;
        queueInfo->queueFamilyIndex = this->deviceInfo.queueFamilies.indices[f];
        queueInfo->queueCount = 1;
        queueInfo->pQueuePriorities = &queuePriority;
    }

    memset(&features, 0, sizeof(features));
    createInfo.queueCreateInfoCount = this->deviceInfo.queueFamilies.count;
    createInfo.pQueueCreateInfos = queueInfos;
    createInfo.pEnabledFeatures = &features;

    FLUX_LOG_DEBUG("Creating logical device...");
    result = this->rhi->dispatch.vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->device);
    if (result != VK_SUCCESS)  {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create logical device");
        return result;
    }

    VulkanLoader::loadDeviceSymbols(&this->rhi->dispatch, this->device, &this->dispatch);

    /* Get queue handles */
    this->dispatch.vkGetDeviceQueue(this->device, this->deviceInfo.queueFamilies.graphicsIndex, 0, &this->deviceInfo.graphicsQueue);
    if (this->deviceInfo.queueFamilies.graphicsIndex == this->deviceInfo.queueFamilies.presentIndex) {
        this->deviceInfo.presentQueue = this->deviceInfo.graphicsQueue;
    } else {
        this->dispatch.vkGetDeviceQueue(this->device, this->deviceInfo.queueFamilies.presentIndex, 0, &this->deviceInfo.presentQueue);
    }

    return VK_SUCCESS;
}

VkResult VulkanDevice::createSwapchain(Window window) {
    VkResult result;

    static VkSwapchainCreateInfoKHR swapchainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = VK_NULL_HANDLE,
        .minImageCount = 0,
        .imageFormat = VK_FORMAT_MAX_ENUM,
        .imageColorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR,
        .imageExtent = {
            .width = 0,
            .height = 0
        },
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_MAX_ENUM,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = VK_SURFACE_TRANSFORM_FLAG_BITS_MAX_ENUM_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE,
    };

    this->selectSwapchainParameters(window);
    swapchainCreateInfo.surface = this->surface;
    swapchainCreateInfo.minImageCount = this->swapchainImageCount;
    swapchainCreateInfo.imageFormat = this->swapchainImageFormat->format;
    swapchainCreateInfo.imageColorSpace = this->swapchainImageFormat->colorSpace;
    swapchainCreateInfo.imageExtent = this->swapchainImageExtent;
    swapchainCreateInfo.preTransform = this->deviceInfo.surface.capabilities.currentTransform;
    swapchainCreateInfo.presentMode = this->swapchainPresentMode;

    /* Share queue families if needed */
    if (this->deviceInfo.queueFamilies.count > 1) {
        FLUX_LOG_DEBUG("Image sharing mode: concurrent");
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = this->deviceInfo.queueFamilies.count;
        swapchainCreateInfo.pQueueFamilyIndices = this->deviceInfo.queueFamilies.indices;
    } else {
        FLUX_LOG_DEBUG("Image sharing mode: exclusive");
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    FLUX_LOG_DEBUG("Creating swapchain...");
    result = this->dispatch.vkCreateSwapchainKHR(this->device, &swapchainCreateInfo, nullptr, &this->swapchain);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create swapchain");
        return result;
    }

    return VK_SUCCESS;
}

VkResult VulkanDevice::createRenderPass(void) {
    VkResult result;

    static VkAttachmentDescription colorAttachmentDescription = {
        .flags = 0,
        .format = VK_FORMAT_MAX_ENUM,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    static const VkAttachmentReference colorAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    static const VkSubpassDescription subpassDescription = {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = nullptr,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    static const VkSubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = 0,
    };

    static const VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = 1,
        .pAttachments = &colorAttachmentDescription,
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency,
    };

    colorAttachmentDescription.format = this->swapchainImageFormat->format;

    /* TODO: Use vkCreateRenderPass2(), dynamic rendering */
    FLUX_LOG_DEBUG("Creating render pass...");
    result = this->dispatch.vkCreateRenderPass(this->device, &renderPassCreateInfo, nullptr, &this->renderPass);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create render pass");
        return result;
    }

    return VK_SUCCESS;
}

VkResult VulkanDevice::createSwapchainFramebuffers(void) {
    VkResult result;
    uint32_t imageCount;
    VkImage *images;
    VkImageView attachment;

    static VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = VK_NULL_HANDLE,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_MAX_ENUM,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };

    static VkFramebufferCreateInfo framebufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderPass = 0,
        .attachmentCount = 0,
        .pAttachments = nullptr,
        .width = UINT32_MAX,
        .height = UINT32_MAX,
        .layers = 1,
    };

    result = getSwapchainImages(&imageCount, &images);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to get swapchain images");
        return result;
    }
    this->swapchainImageCount = imageCount;
    FLUX_LOG_DEBUG("Final image count: %u", this->swapchainImageCount);

    this->swapchainImageViews = new(std::nothrow) VkImageView[this->swapchainImageCount];
    if (this->swapchainImageViews == nullptr) {
        FLUX_LOG_ERROR("Failed to allocate memory for swapchain image views");
        delete[] images;
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    FLUX_LOG_DEBUG("Creating swapchain image views...");
    imageViewCreateInfo.format = this->swapchainImageFormat->format;
    for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
        imageViewCreateInfo.image = images[i];
        result = this->dispatch.vkCreateImageView(this->device, &imageViewCreateInfo, nullptr, &this->swapchainImageViews[i]);
        if (result != VK_SUCCESS) {
            FLUX_LOG_VULKAN_ERROR(result, "Failed to create swapchain image view");

            /* Destroy previously created views */
            for (uint32_t v = 0; v < i; v++) {
                this->dispatch.vkDestroyImageView(this->device, this->swapchainImageViews[v], nullptr);
            }

            delete[] this->swapchainImageViews;
            this->swapchainImageViews = nullptr;
            delete[] images;
            return result;
        }
    }
    delete[] images;

    this->swapchainFramebuffers = new(std::nothrow) VkFramebuffer[imageCount];
    if (this->swapchainFramebuffers == nullptr) {
        FLUX_LOG_ERROR("Failed to allocate memory for swapchain framebuffers");
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    FLUX_LOG_DEBUG("Creating swapchain framebuffers...");
    framebufferCreateInfo.renderPass = this->renderPass;
    framebufferCreateInfo.attachmentCount = 1;
    framebufferCreateInfo.pAttachments = &attachment;
    framebufferCreateInfo.width = this->swapchainImageExtent.width;
    framebufferCreateInfo.height = this->swapchainImageExtent.height;
    for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
        attachment = this->swapchainImageViews[i];
        result = this->dispatch.vkCreateFramebuffer(this->device, &framebufferCreateInfo, nullptr, &this->swapchainFramebuffers[i]);
        if (result != VK_SUCCESS) {
            FLUX_LOG_VULKAN_ERROR(result, "Failed to create swapchain framebuffer");

            /* Destroy previously created framebuffers */
            for (uint32_t f = 0; f < i; f++) {
                dispatch.vkDestroyFramebuffer(this->device, this->swapchainFramebuffers[f], nullptr);
            }

            delete[] this->swapchainFramebuffers;
            this->swapchainFramebuffers = nullptr;
            return result;
        }
    }

    return VK_SUCCESS;
}

VkResult VulkanDevice::createAll(Window window) {
    VkResult result;

    result = this->createSurface(window);
    if (result != VK_SUCCESS) {
        return result;
    }

    result = this->createDevice();
    if (result != VK_SUCCESS) {
        return result;
    }

    result = this->createSwapchain(window);
    if (result != VK_SUCCESS) {
        return result;
    }

    result = this->createRenderPass();
    if (result != VK_SUCCESS) {
        return result;
    }

    result = this->createSwapchainFramebuffers();
    if (result != VK_SUCCESS) {
        return result;
    }

    return VK_SUCCESS;
}

Status VulkanDevice::create(VulkanRHI *rhi, Window window) {
    VkResult result;

    /* In case destroy() is needed before we finish */
    this->surface = VK_NULL_HANDLE;
    this->deviceInfo.surface.formats = nullptr;
    this->deviceInfo.surface.presentModes = nullptr;
    this->device = VK_NULL_HANDLE;
    this->swapchain = VK_NULL_HANDLE;
    this->renderPass = VK_NULL_HANDLE;

    this->rhi = rhi;
    result = this->createAll(window);
    if (result != VK_SUCCESS) {
        this->destroy();
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

Status VulkanDevice::createPipeline(const RHIPipelineDescription *description, RHIPipeline **pipelineOut) {
    Status status;
    VulkanPipeline *pipeline;

    pipeline = new(std::nothrow) VulkanPipeline();
    if (pipeline == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = pipeline->create(this, description);
    if (status != Status::success) {
        delete pipeline;
        return status;
    }

    *pipelineOut = pipeline;
    return Status::success;
}

Status VulkanDevice::createCommandPool(RHICommandPool **poolOut) {
    Status status;
    VulkanCommandPool *pool;

    pool = new(std::nothrow) VulkanCommandPool();
    if (pool == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = pool->create(this);
    if (status != Status::success) {
        delete pool;
        return status;
    }

    *poolOut = pool;
    return Status::success;
}

Status VulkanDevice::createRenderingAgent(RHIRenderingAgent **agentOut) {
    Status status;
    VulkanRenderingAgent *agent;

    agent = new(std::nothrow) VulkanRenderingAgent();
    if (agent == nullptr) {
        return Status::hostAllocationFailed;
    }

    status = agent->create(this);
    if (status != Status::success) {
        delete agent;
        return status;
    }

    *agentOut = agent;
    return Status::success;
}

}
