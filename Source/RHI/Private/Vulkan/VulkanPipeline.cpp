/*
 * Vulkan RHI pipeline.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "Log.hpp"
#include "VulkanLog.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanResult.hpp"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

namespace Flux {

VkPrimitiveTopology VulkanPipeline::translatePrimitiveTopology(RHIPrimitiveTopology topology) {
    switch (topology) {
    case RHIPrimitiveTopology::pointList:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case RHIPrimitiveTopology::lineList:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case RHIPrimitiveTopology::lineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case RHIPrimitiveTopology::triangleList:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case RHIPrimitiveTopology::triangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    case RHIPrimitiveTopology::triangleFan:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    case RHIPrimitiveTopology::lineListWithAdjacency:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
    case RHIPrimitiveTopology::lineStripWithAdjacency:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
    case RHIPrimitiveTopology::triangleListWithAdjacency:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
    case RHIPrimitiveTopology::triangleStripWithAdjacency:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
    case RHIPrimitiveTopology::patchList:
        return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
    default:
        return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    }
}

VkShaderStageFlagBits VulkanPipeline::translateShaderStageType(RHIShaderStageType type) {
    switch (type) {
    case RHIShaderStageType::vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case RHIShaderStageType::tessellationControl:
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case RHIShaderStageType::tessellationEvaluation:
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case RHIShaderStageType::geometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case RHIShaderStageType::fragment:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case RHIShaderStageType::compute:
        return VK_SHADER_STAGE_COMPUTE_BIT;
    default:
        return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
}

VkVertexInputRate VulkanPipeline::translateVertexInputRate(RHIVertexInputRate rate) {
    switch (rate) {
    case RHIVertexInputRate::vertex:
        return VK_VERTEX_INPUT_RATE_VERTEX;
    case RHIVertexInputRate::instance:
        return VK_VERTEX_INPUT_RATE_INSTANCE;
    default:
        return VK_VERTEX_INPUT_RATE_MAX_ENUM;
    }
}

VkFormat VulkanPipeline::translateComponentFormat(RHIComponentType componentType, uint32_t componentCount) {
    switch (componentType) {
    case RHIComponentType::float32:
        switch (componentCount) {
        case 1:
            return VK_FORMAT_R32_SFLOAT;
        case 2:
            return VK_FORMAT_R32G32_SFLOAT;
        case 3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            return VK_FORMAT_MAX_ENUM;
        }
    default:
        return VK_FORMAT_MAX_ENUM;
    }
}

void VulkanPipeline::destroyShaderStageCreateInfos(uint32_t stageCount, VkPipelineShaderStageCreateInfo *infos) {
    for (uint32_t i = 0; i < stageCount; i++) {
        this->device->dispatch.vkDestroyShaderModule(this->device->device, infos[i].module, nullptr);
    }

    delete[] infos;
}

VkResult VulkanPipeline::translateShaderStageDescriptions(uint32_t stageCount, const RHIShaderStageDescription *descriptions, VkPipelineShaderStageCreateInfo **createInfosOut) {
    VkResult result;
    const RHIShaderStageDescription *description;
    VkPipelineShaderStageCreateInfo *createInfos, *createInfo;

    static VkShaderModuleCreateInfo moduleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = 0,
        .pCode = nullptr,
    };

    createInfos = new(std::nothrow) VkPipelineShaderStageCreateInfo[stageCount];
    if (createInfos == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    /* Translate descriptions to create infos */
    description = &descriptions[0];
    createInfo = &createInfos[0];
    for (uint32_t s = 0; s < stageCount; s++) {
        createInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo->pNext = nullptr;
        createInfo->flags = 0;
        createInfo->stage = translateShaderStageType(description->type);
        createInfo->pName = description->entryPointName;
        createInfo->pSpecializationInfo = nullptr;
        if (createInfo->stage == VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM) {
            FLUX_LOG_ERROR("Invalid shader stage type 0x%08x", (uint32_t) description->type);
            this->destroyShaderStageCreateInfos(s, createInfos);
            return VK_ERROR_UNKNOWN;
        }

        /* Create shader module */
        moduleCreateInfo.codeSize = description->codeSize;
        moduleCreateInfo.pCode = (const uint32_t *) description->code;
        result = this->device->dispatch.vkCreateShaderModule(this->device->device, &moduleCreateInfo, nullptr, &createInfo->module);
        if (result != VK_SUCCESS) {
            FLUX_LOG_VULKAN_ERROR(result, "Failed to create shader module");
            this->destroyShaderStageCreateInfos(s, createInfos);
            return result;
        }

        description++;
        createInfo++;
    }

    *createInfosOut = createInfos;
    return VK_SUCCESS;
}

VkResult VulkanPipeline::translateVertexInputBindingDescriptions(uint32_t bindingCount, const RHIVertexInputBindingDescription *bindingDescriptions, VkPipelineVertexInputStateCreateInfo *vertexInputStateCreateInfo) {
    uint32_t totalAttributeCount;
    const RHIVertexInputBindingDescription *fluxBindingDescription;
    const RHIVertexInputAttributeDescription *fluxAttributeDescription;
    VkVertexInputBindingDescription *vulkanBindingDescriptions, *vulkanBindingDescription;
    VkVertexInputAttributeDescription *vulkanAttributeDescriptions, *vulkanAttributeDescription;

    /* Vulkan needs all the attributes in a flat array */
    totalAttributeCount = 0;
    fluxBindingDescription = &bindingDescriptions[0];
    for (uint32_t b = 0; b < bindingCount; b++) {
        totalAttributeCount += fluxBindingDescription->attributeCount;
        fluxBindingDescription++;
    }

    vulkanBindingDescriptions = new(std::nothrow) VkVertexInputBindingDescription[bindingCount];
    if (vulkanBindingDescriptions == nullptr) {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    vulkanAttributeDescriptions = new(std::nothrow) VkVertexInputAttributeDescription[totalAttributeCount];
    if (vulkanAttributeDescriptions == nullptr) {
        delete[] vulkanBindingDescriptions;
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    /* Translate bindings */
    fluxBindingDescription = &bindingDescriptions[0];
    vulkanBindingDescription = &vulkanBindingDescriptions[0];
    vulkanAttributeDescription = &vulkanAttributeDescriptions[0];
    for (uint32_t b = 0; b < bindingCount; b++) {
        vulkanBindingDescription->binding = b;
        vulkanBindingDescription->stride = fluxBindingDescription->stride;
        vulkanBindingDescription->inputRate = translateVertexInputRate(fluxBindingDescription->inputRate);
        if (vulkanBindingDescription->inputRate == VK_VERTEX_INPUT_RATE_MAX_ENUM) {
            FLUX_LOG_ERROR("Invalid vertex input rate 0x%08x", fluxBindingDescription->inputRate);
            delete[] vulkanAttributeDescriptions;
            delete[] vulkanBindingDescriptions;
            return VK_ERROR_UNKNOWN;
        }

        /* Translate attributes */
        fluxAttributeDescription = &fluxBindingDescription->attributeDescriptions[0];
        for (uint32_t a = 0; a < fluxBindingDescription->attributeCount; a++) {
            vulkanAttributeDescription->location = fluxAttributeDescription->location;
            vulkanAttributeDescription->binding = b;
            vulkanAttributeDescription->format = translateComponentFormat(fluxAttributeDescription->componentType, fluxAttributeDescription->componentCount);
            vulkanAttributeDescription->offset = fluxAttributeDescription->offset;
            if (vulkanAttributeDescription->format == VK_FORMAT_MAX_ENUM) {
                FLUX_LOG_ERROR("Invalid component format (type 0x%08x, count %u)", (uint32_t) fluxAttributeDescription->componentType, fluxAttributeDescription->componentCount);
                delete[] vulkanAttributeDescriptions;
                delete[] vulkanBindingDescriptions;
                return VK_ERROR_UNKNOWN;
            }

            fluxAttributeDescription++;
            vulkanAttributeDescription++;
        }

        fluxBindingDescription++;
        vulkanBindingDescription++;
    }

    vertexInputStateCreateInfo->vertexBindingDescriptionCount = bindingCount;
    vertexInputStateCreateInfo->pVertexBindingDescriptions = vulkanBindingDescriptions;
    vertexInputStateCreateInfo->vertexAttributeDescriptionCount = totalAttributeCount;
    vertexInputStateCreateInfo->pVertexAttributeDescriptions = vulkanAttributeDescriptions;
    return VK_SUCCESS;
}

void VulkanPipeline::destroy(void) {
    if (this->pipeline != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying graphics pipeline...");
        this->device->dispatch.vkDestroyPipeline(this->device->device, this->pipeline, nullptr);
        this->pipeline = VK_NULL_HANDLE;
    }

    if (this->layout != VK_NULL_HANDLE) {
        FLUX_LOG_DEBUG("Destroying pipeline layout...");
        this->device->dispatch.vkDestroyPipelineLayout(this->device->device, this->layout, nullptr);
        this->layout = VK_NULL_HANDLE;
    }
}

VkResult VulkanPipeline::createLayout(void) {
    VkResult result;

    static const VkPipelineLayoutCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    FLUX_LOG_DEBUG("Creating pipeline layout...");
    result = this->device->dispatch.vkCreatePipelineLayout(this->device->device, &createInfo, nullptr, &this->layout);
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create pipeline layout");
        return result;
    }

    return VK_SUCCESS;
}

VkResult VulkanPipeline::createPipeline(const RHIPipelineDescription *description) {
    VkResult result;
    VkPipelineShaderStageCreateInfo *shaderStageCreateInfos;

    static VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = nullptr,
    };

    static VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,
        .primitiveRestartEnable = VK_FALSE,
    };

    static const VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = nullptr,
        .scissorCount = 1,
        .pScissors = nullptr,
    };

    static const VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    static const VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 0.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };

    static const VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    static const VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachmentState,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
    };

    static const VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    static const VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = ARRAY_SIZE(dynamicStates),
        .pDynamicStates = dynamicStates,
    };

    static VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stageCount = 0,
        .pStages = nullptr,
        .pVertexInputState = &vertexInputStateCreateInfo,
        .pInputAssemblyState = &inputAssemblyStateCreateInfo,
        .pTessellationState = nullptr,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = this->layout,
        .renderPass = this->device->renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0,
    };

    inputAssemblyStateCreateInfo.topology = translatePrimitiveTopology(description->primitiveTopology);
    if (inputAssemblyStateCreateInfo.topology == VK_PRIMITIVE_TOPOLOGY_MAX_ENUM) {
        FLUX_LOG_ERROR("Invalid primitive topology 0x%08x", (uint32_t) description->primitiveTopology);
        return VK_ERROR_UNKNOWN;
    }

    /* Translate shader stage descriptions, if any */
    shaderStageCreateInfos = nullptr;
    if (description->shaderStageCount >= 1) {
        result = this->translateShaderStageDescriptions(description->shaderStageCount, description->shaderStageDescriptions, &shaderStageCreateInfos);
        if (result != VK_SUCCESS) {
            return result;
        }

        pipelineCreateInfo.stageCount = description->shaderStageCount;
        pipelineCreateInfo.pStages = shaderStageCreateInfos;
    }

    /* Translate vertex input binding descriptions, if any */
    if (description->vertexInputBindingCount >= 1) {
        result = this->translateVertexInputBindingDescriptions(description->vertexInputBindingCount, description->vertexInputBindingDescriptions, &vertexInputStateCreateInfo);
        if (result != VK_SUCCESS) {
            this->destroyShaderStageCreateInfos(description->shaderStageCount, shaderStageCreateInfos);
            return result;
        }
    }

    FLUX_LOG_DEBUG("Creating graphics pipeline...");
    result = this->device->dispatch.vkCreateGraphicsPipelines(this->device->device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->pipeline);

    /* Always do cleanup */
    delete[] vertexInputStateCreateInfo.pVertexAttributeDescriptions;
    delete[] vertexInputStateCreateInfo.pVertexBindingDescriptions;
    this->destroyShaderStageCreateInfos(description->shaderStageCount, shaderStageCreateInfos);

    /* Now handle errors */
    if (result != VK_SUCCESS) {
        FLUX_LOG_VULKAN_ERROR(result, "Failed to create graphics pipeline");
        return result;
    }

    return VK_SUCCESS;
}

Status VulkanPipeline::create(VulkanDevice *device, const RHIPipelineDescription *description) {
    VkResult result;

    this->device = device;

    /* In case destroy() is needed before we finish */
    this->layout = VK_NULL_HANDLE;
    this->pipeline = VK_NULL_HANDLE;

    result = this->createLayout();
    if (result != VK_SUCCESS) {
        return VulkanResult::getStatus(result);
    }

    result = this->createPipeline(description);
    if (result != VK_SUCCESS) {
        this->destroy();
        return VulkanResult::getStatus(result);
    }

    return Status::success;
}

}
