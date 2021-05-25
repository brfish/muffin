#ifndef _MUFFIN_RENDER_PIPELINE_H_
#define _MUFFIN_RENDER_PIPELINE_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"
#include "muffin_render/buffer.h"
#include "muffin_render/shader.h"

typedef enum MufColorMask_e {
    MUF_COLOR_MASK_NONE,
    MUF_COLOR_MASK_R,
    MUF_COLOR_MASK_G,
    MUF_COLOR_MASK_B,
    MUF_COLOR_MASK_A,
    MUF_COLOR_MASK_RG,
    MUF_COLOR_MASK_RB,
    MUF_COLOR_MASK_RA,
    MUF_COLOR_MASK_GB,
    MUF_COLOR_MASK_GA,
    MUF_COLOR_MASK_BA,
    MUF_COLOR_MASK_RGB,
    MUF_COLOR_MASK_GBA,
    MUF_COLOR_MASK_RGBA,
} MufColorMask;

typedef enum MufCompareOperator_e {
    MUF_COMPARE_OP_ALWAYS,
    MUF_COMPARE_OP_NEVER,
    MUF_COMPARE_OP_LESS,
    MUF_COMPARE_OP_GREATER,
    MUF_COMPARE_OP_EQUAL,
    MUF_COMPARE_OP_NEQUAL,
    MUF_COMPARE_OP_LEQAUL,
    MUF_COMPARE_OP_GEQUAL
} MufCompareOperator;

typedef enum MufPrimitiveType_e {
    MUF_PRIMITIVE_TYPE_POINTS,
    MUF_PRIMITIVE_TYPE_LINES,
    MUF_PRIMITIVE_TYPE_LINE_STRIP,
    MUF_PRIMITIVE_TYPE_TRIANGLES,
    MUF_PRIMITIVE_TYPE_TRIANGLE_STRIP,
} MufPrimitiveType;

typedef enum MufPolygonMode_e {
    MUF_POLYGON_MODE_POINT,
    MUF_POLYGON_MODE_LINE,
    MUF_POLYGON_MODE_FILL
} MufPolygonMode;

typedef enum MufCullModeFlags_e {
    MUF_CULL_MODE_BACK          = 0x01,
    MUF_CULL_MODE_FRONT         = 0x02,
    MUF_CULL_MODE_FRONT_BACK    = 0x03
} MufCullModeFlags;

typedef enum MufFrontFace_e {
    MUF_FRONT_FACE_CW,
    MUF_FRONT_FACE_CCW
} MufFrontFace;

typedef enum MufStencilOperator_e {
    MUF_STENCIL_OP_KEEP,
    MUF_STENCIL_OP_ZERO,
    MUF_STENCIL_OP_REPLACE,
    MUF_STENCIL_OP_INC,
    MUF_STENCIL_OP_WARP_INC,
    MUF_STENCIL_OP_DEC,
    MUF_STENCIL_OP_WRAP_DEC,
    MUF_STENCIL_OP_INVERT
} MufStencilOperator;

typedef enum MufBlendFactor_e {
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
    MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA
} MufBlendFactor;

typedef enum MufBlendOperator_e {
    MUF_BLEND_OP_ADD,
    MUF_BLEND_OP_SUB,
    MUF_BLEND_OP_REVERSE_SUB
} MufBlendOperator;

typedef struct MufVertexInputAttribute_s {
    MufFormat   format;
    muf_u32     elementCount;
    muf_u32     location;
    MufBuffer   buffer;
    muf_usize   offset;
    muf_usize   stride;
} MufVertexInputAttribute;

typedef struct MufVertexInputLayout_s {
    muf_u32                         attributeCount;
    const MufVertexInputAttribute   *attributes;
} MufVertexInputLayout;

typedef struct MufViewport_s {
    muf_f32 x;
    muf_f32 y;
    muf_f32 width;
    muf_f32 height;
    muf_f32 minDepth;
    muf_f32 maxDepth;
} MufViewport;

typedef MufRect2 MufScissor;

typedef struct MufDepthBiasDesc_s {
    muf_f32 constantFactor;
    muf_f32 clamp;
    muf_f32 slopeFactor;
} MufDepthBiasDesc;

typedef struct MufRasterizerStateCreateInfo_s {
    MufCullModeFlags        cullMode;
    MufFrontFace            frontFace;
    MufPolygonMode          polygonMode;
    muf_f32                 lineWidth;
    muf_bool                depthClampEnabled;
    const MufDepthBiasDesc  *depthBias;
    muf_bool                multiSamplingEnabled;
    muf_u32                 multiSampleCount;
} MufRasterizerStateCreateInfo;

typedef struct MufDepthStateCreateInfo_s {
    muf_bool            testEnabled;
    muf_bool            writeEnabled;
    MufCompareOperator  compareOp;
} MufDepthStateCreateInfo;

typedef struct MufStencilStateCreateInfo_s {
    muf_bool            testEnabled;
    MufStencilOperator  failOp;
    MufStencilOperator  passOp;
    MufStencilOperator  depthFailOp;
    MufCompareOperator  compareOp;
    muf_u32             compareMask;
    muf_u32             writeMask;
    muf_u32             ref;
} MufStencilStateCreateInfo;

typedef struct MufBlendStateCreateInfo_s {
    MufBlendFactor      srcColorFactor;
    MufBlendFactor      dstColorFactor;
    MufBlendOperator    colorBlendOp;
    MufBlendFactor      srcAlphaFactor;
    MufBlendFactor      dstAlphaFactor;
    MufBlendOperator    alphaBlendOp;
    MufRGBA             blendConstant;
} MufBlendStateCreateInfo;

typedef struct MufPipelineCreateInfo_s {
    const MufShaderProgram                *shaderProgram;
    const MufVertexInputLayout            *inputLayout;
    const MufViewport                     *viewport;
    const MufScissor                      *scissor;
    const MufRasterizerStateCreateInfo    *rasterizer;
    const MufDepthStateCreateInfo         *depth;
    const MufStencilStateCreateInfo       *stencil;
    const MufBlendStateCreateInfo         *blend;
} MufPipelineCreateInfo;

typedef MufHandle MufPipeline;

MUF_API MufPipeline mufCreatePipeline(const MufPipelineCreateInfo *info);
MUF_API void mufDestroyPipeline(MufPipeline pipeline);
MUF_API void mufPipelineSetViewport(MufPipeline pipeline, const MufViewport *viewport);

#endif