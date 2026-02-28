/*
 * Vulkan RHI pipeline.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "VulkanDevice.hpp"

namespace Flux {

class VulkanPipeline: public RHIPipeline {
private:

    void destroyShaderStageCreateInfos(uint32_t stageCount, VkPipelineShaderStageCreateInfo *infos);

    VkResult translateShaderStageDescriptions(uint32_t stageCount, const RHIShaderStageDescription *descriptions, VkPipelineShaderStageCreateInfo **createInfos);

    VkResult translateVertexInputBindingDescriptions(uint32_t bindingCount, const RHIVertexInputBindingDescription *bindingDescriptions, VkPipelineVertexInputStateCreateInfo *vertexInputStateCreateInfo);

    VkResult createLayout(void);

    VkResult createPipeline(const RHIPipelineDescription *description);

public:

    VulkanDevice &device;

    VkPipelineLayout layout;
    VkPipeline pipeline;

    static VkPrimitiveTopology translatePrimitiveTopology(RHIPrimitiveTopology topology);

    static VkShaderStageFlagBits translateShaderStageType(RHIShaderStageType type);

    static VkVertexInputRate translateVertexInputRate(RHIVertexInputRate rate);

    static VkFormat translateComponentFormat(RHIComponentType componentType, uint32_t componentCount);

    VulkanPipeline(VulkanDevice &device);

    Status create(const RHIPipelineDescription *description);

    virtual void destroy(void);

};

}
