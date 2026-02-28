/*
 * RHI pipeline.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace Flux {

enum class RHIPrimitiveTopology {
    pointList,
    lineList,
    lineStrip,
    triangleList,
    triangleStrip,
    triangleFan,
    lineListWithAdjacency,
    lineStripWithAdjacency,
    triangleListWithAdjacency,
    triangleStripWithAdjacency,
    patchList,
};

enum class RHIShaderStageType {
    vertex,
    tessellationControl,
    tessellationEvaluation,
    geometry,
    fragment,
    compute,
};

enum class RHIVertexInputRate {
    vertex,
    instance,
};

enum class RHIComponentType {
    float32,
};

typedef struct {
    RHIShaderStageType type;
    size_t codeSize;
    const void *code;
    const char *entryPointName;
} RHIShaderStageDescription;

typedef struct {
    uint32_t location;
    RHIComponentType componentType;
    uint32_t componentCount;
    uint32_t offset;
} RHIVertexInputAttributeDescription;

typedef struct {
    uint32_t stride;
    RHIVertexInputRate inputRate;
    uint32_t attributeCount;
    const RHIVertexInputAttributeDescription *attributeDescriptions;
} RHIVertexInputBindingDescription;

typedef struct {
    RHIPrimitiveTopology primitiveTopology;

    uint32_t shaderStageCount;
    const RHIShaderStageDescription *shaderStageDescriptions;

    uint32_t vertexInputBindingCount;
    const RHIVertexInputBindingDescription *vertexInputBindingDescriptions;
} RHIPipelineDescription;

class RHIPipeline {
public:

    virtual void destroy(void) = 0;

};

}
