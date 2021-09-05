#ifndef _MUFFIN_RENDER_PIPELINE_H_
#define _MUFFIN_RENDER_PIPELINE_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"
#include "muffin_render/enums.h"
#include "muffin_render/resources.h"

typedef enum MufPrimitiveTopology_e {
    MUF_ENUM_UNKNOWN(MUF_PRIMITIVE_TOPOLOGY),
    MUF_PRIMITIVE_TOPOLOGY_POINTS,
    MUF_PRIMITIVE_TOPOLOGY_LINES,
    MUF_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    MUF_PRIMITIVE_TOPOLOGY_TRIANGLES,
    MUF_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    MUF_ENUM_COUNT(MUF_PRIMITIVE_TOPOLOGY)
} MufPrimitiveTopology;

typedef enum MufPolygonMode_e {
    MUF_ENUM_UNKNOWN(MUF_POLYGON_MODE),
    MUF_POLYGON_MODE_POINT,
    MUF_POLYGON_MODE_LINE,
    MUF_POLYGON_MODE_FILL,
    MUF_ENUM_COUNT(MUF_POLYGON_MODE)
} MufPolygonMode;

typedef enum MufCullModeFlags_e {
    MUF_CULL_MODE_FLAGS_NONE        = 0X0000,
    MUF_CULL_MODE_FLAGS_BACK        = 0x0001,
    MUF_CULL_MODE_FLAGS_FRONT       = 0x0002,
    MUF_CULL_MODE_FLAGS_FRONT_BACK  = MUF_CULL_MODE_FLAGS_BACK | MUF_CULL_MODE_FLAGS_FRONT
} MufCullModeFlags;

typedef enum MufFrontFace_e {
    MUF_ENUM_UNKNOWN(MUF_FRONT_FACE),
    MUF_FRONT_FACE_CW,
    MUF_FRONT_FACE_CCW,
    MUF_ENUM_COUNT(MUF_FRONT_FACE)
} MufFrontFace;

typedef enum MufStencilOperator_e {
    MUF_ENUM_UNKNOWN(MUF_STENCIL_OP),
    MUF_STENCIL_OP_KEEP,
    MUF_STENCIL_OP_ZERO,
    MUF_STENCIL_OP_REPLACE,
    MUF_STENCIL_OP_INC,
    MUF_STENCIL_OP_WARP_INC,
    MUF_STENCIL_OP_DEC,
    MUF_STENCIL_OP_WRAP_DEC,
    MUF_STENCIL_OP_INVERT,
    MUF_ENUM_COUNT(MUF_STENCIL_OP)
} MufStencilOperator;

typedef enum MufBlendFactor_e {
    MUF_ENUM_UNKNOWN(MUF_BLEND_FACTOR),
    MUF_BLEND_FACTOR_ZERO,
    MUF_BLEND_FACTOR_ONE,
    MUF_BLEND_FACTOR_SRC_COLOR,
    MUF_BLEND_FACTOR_DST_COLOR,
    MUF_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    MUF_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    MUF_BLEND_FACTOR_SRC_ALPHA,
    MUF_BLEND_FACTOR_DST_ALPHA,
    MUF_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    MUF_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    MUF_BLEND_FACTOR_CONSTANT,
    MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT,
    MUF_BLEND_FACTOR_CONSTANT_ALPHA,
    MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    MUF_ENUM_COUNT(MUF_BLEND_FACTOR)
} MufBlendFactor;

typedef enum MufBlendOperator_e {
    MUF_ENUM_UNKNOWN(MUF_BLEND_OP),
    MUF_BLEND_OP_ADD,
    MUF_BLEND_OP_SUB,
    MUF_BLEND_OP_REVERSE_SUB,
    MUF_ENUM_COUNT(MUF_BLEND_OP)
} MufBlendOperator;

typedef struct MufVertexInputAttribute_s {
    MufFormat   format;
    muf_u32     location;
    muf_u32     bufferIndex;
    muf_usize   offset;
} MufVertexInputAttribute;

typedef struct MufVertexInputLayout_s {
    muf_u32                         attributeCount;
    const MufVertexInputAttribute   *attributes;
    muf_usize                       stride;
} MufVertexInputLayout;

typedef struct MufViewport_s {
    muf_u32 x;
    muf_u32 y;
    muf_u32 width;
    muf_u32 height;
    muf_f32 minDepth;
    muf_f32 maxDepth;
} MufViewport;

typedef MufRect2i MufScissor;

typedef struct MufDepthBiasDesc_s {
    muf_bool    enabled;
    muf_f32     constantFactor;
    muf_f32     clamp;
    muf_f32     slopeFactor;
} MufDepthBiasDesc;

typedef struct MufRasterizerStateCreateInfo_s {
    MufCullModeFlags        cullMode;
    MufFrontFace            frontFace;
    MufPolygonMode          polygonMode;
    muf_f32                 lineWidth;
    muf_bool                depthClampEnabled;
    MufDepthBiasDesc        depthBias;
    muf_bool                multiSampleEnabled;
    muf_u32                 multiSampleCount;
} MufRasterizerStateCreateInfo;

#define MUF_DEFAULT_RASTERIZER_STATE_CREATE_INFO = (MufRasterizerStateCreateInfo) { \
    .cullMode = MUF_CULL_MODE_FLAGS_BACK, \
    .frontFace = MUF_FRONT_FACE_CCW, \
    .polygonMode = MUF_POLYGON_MODE_FILL, \
    .lineWidth = 1.0F, \
    .depthClampEnabled = MUF_FALSE, \
    .depthBiasEnabled = MUF_FALSE, \
    .multiSampleEnabled = MUF_FALSE, \
    .multiSampleCount = MUF_SAMPLE_COUNT_1 \
}

typedef struct MufDepthStateCreateInfo_s {
    muf_bool            testEnabled;
    muf_bool            writeEnabled;
    MufCompareOperator  compareOp;
} MufDepthStateCreateInfo;

#define MUF_DEFAULT_DEPTH_STATE_CREATE_INFO (MufDepthStateCreateInfo) { \
    .testEnabled = MUF_FALSE, \
    .writeEnabled = MUF_FALSE, \
    .compareOp = MUF_COMPARE_OP_ALWAYS \
}

typedef struct MufStencilOpState_s {
    MufStencilOperator  failOp;
    MufStencilOperator  passOp;
    MufStencilOperator  depthFailOp;
    MufCompareOperator  compareOp;
    muf_u32             writeMask;
    muf_u32             compareMask;
    muf_u32             ref;
} MufStencilOpState;

#define MUF_DEFAULT_STENCILOP_STATE (MufStencilOpState) { \
    .failOp = MUF_STENCIL_OP_KEEP, \
    .passOp = MUF_STENCIL_OP_KEEP, \
    .depthFailOp = MUF_STENCIL_OP_KEEP, \
    .compareOp = MUF_COMPARE_OP_ALWAYS, \
    .writeMask = 0, \
    .compareMask = 0, \
    .ref = 0 \
}

typedef struct MufStencilStateCreateInfo_s {
    muf_bool            testEnabled;
    MufStencilOpState   front;
    MufStencilOpState   back;
} MufStencilStateCreateInfo;

#define MUF_DEFAULT_STENCIL_STATE_CREATE_INFO (MufStencilStateCreateInfo) { \
    .testEnabled = MUF_FALSE, \
    .front = MUF_DEFAULT_STENCILOP_STATE, \
    .back = MUF_DEFAULT_STENCILOP_STATE \
}

typedef struct MufBlendStateCreateInfo_s {
    muf_bool            enabled;
    MufBlendFactor      srcColorFactor;
    MufBlendFactor      dstColorFactor;
    MufBlendOperator    colorBlendOp;
    MufBlendFactor      srcAlphaFactor;
    MufBlendFactor      dstAlphaFactor;
    MufBlendOperator    alphaBlendOp;
    MufRGBA             constant;
} MufBlendStateCreateInfo;

#define MUF_DEFAULT_BLEND_STATE_CREATE_INFO (MufBlendStateCreateInfo) { \
    .enabled = MUF_FALSE, \
    .srcColorFactor = MUF_BLEND_FACTOR_ZERO, \
    .dstColorFactor = MUF_BLEND_FACTOR_ZERO, \
    .colorBlendOp = MUF_BLEND_OP_ADD, \
    .srcAlphaFactor = MUF_BLEND_FACTOR_ZERO, \
    .dstAlphaFactor = MUF_BLEND_FACTOR_ZERO, \
    .alphaBlendOp = MUF_BLEND_OP_ADD, \
    .constant = (MufRGBA) { 0.0F, 0.0F, 0.0F, 0.0F } \
}

typedef struct MufPipelineCreateInfo_s {
    MufShaderProgram                      shaderProgram;
    MufPrimitiveTopology                      primitiveType;
    const MufVertexInputLayout            *inputLayout;
    const MufViewport                     *viewport;
    const MufScissor                      *scissor;
    const MufRasterizerStateCreateInfo    *rasterizer;
    const MufDepthStateCreateInfo         *depth;
    const MufStencilStateCreateInfo       *stencil;
    const MufBlendStateCreateInfo         *blend;
} MufPipelineCreateInfo;

MUF_HANDLE_DEF(MufPipeline);

MUF_API MufPipeline mufCreatePipeline(const MufPipelineCreateInfo *info);
MUF_API void mufDestroyPipeline(MufPipeline pipeline);

#endif