#ifndef _MUFFIN_RENDER_RESOURCES_H_
#define _MUFFIN_RENDER_RESOURCES_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"
#include "muffin_render/enums.h"

typedef enum MufBufferType_e {
    MUF_ENUM_UNKNOWN(MUF_BUFFER_TYPE),
    MUF_BUFFER_TYPE_VERTEX,
    MUF_BUFFER_TYPE_INDEX,
    MUF_BUFFER_TYPE_UNIFORM,
    MUF_BUFFER_TYPE_STORAGE,
    MUF_BUFFER_TYPE_INDIRECT,
    MUF_ENUM_COUNT(MUF_BUFFER_TYPE)
} MufBufferType;

typedef enum MufAccessFlags_e {
    MUF_ACCESS_FLAGS_NONE        = 0x0000,
    MUF_ACCESS_FLAGS_READ        = 0x0001,
    MUF_ACCESS_FLAGS_WRITE       = 0x0002,
    MUF_ACCESS_FLAGS_READ_WRITE  = MUF_ACCESS_FLAGS_READ | MUF_ACCESS_FLAGS_WRITE
} MufAccessFlags;

typedef enum MufBufferFlags_e {
    MUF_BUFFER_FLAGS_NONE       = 0x0000,
    MUF_BUFFER_FLAGS_INDEX16    = 0x0001,
    MUF_BUFFER_FLAGS_DYNAMIC    = 0X0002,
} MufBufferFlags;

typedef struct MufBufferCreateInfo_s {
    MufBufferType       type;
    MufBufferFlags      flags;
    MufAccessFlags      accessFlags;
    muf_usize           size;
    muf_crawptr         data;
} MufBufferCreateInfo;

MUF_HANDLE_DEF(MufBuffer);

MUF_API MufBuffer mufCreateBuffer(const MufBufferCreateInfo *info);
MUF_API void mufDestroyBuffer(MufBuffer buffer);
MUF_API muf_rawptr mufMapBuffer(MufBuffer buffer, muf_offset offset, muf_usize size);
MUF_API void mufUnmapBuffer(MufBuffer buffer);

typedef enum MufTextureType_e {
    MUF_TEXTURE_TYPE_1D,
    MUF_TEXTURE_TYPE_2D,
    MUF_TEXTURE_TYPE_3D
} MufTextureType;

typedef enum MufTextureCompareMode_e {
    MUF_TEXTURE_COMPARE_MODE_NONE,
    MUF_TEXTURE_COMPARE_MODE_REF
} MufTextureCompareMode;

typedef enum MufTextureFilter_e {
    MUF_ENUM_UNKNOWN(MUF_TEXTURE_FILTER),
    MUF_TEXTURE_FILTER_NEAREST,
    MUF_TEXTURE_FILTER_LINEAR,
    MUF_ENUM_COUNT(MUF_TEXTURE_FILTER)
} MufTextureFilter;

typedef enum MufWrapMode_e {
    MUF_ENUM_UNKNOWN(MUF_WRAP_MODE),
    MUF_WRAP_MODE_CLAMP_TO_EDGE,
    MUF_WRAP_MODE_CLAMP_TO_BORDER,
    MUF_WRAP_MODE_MIRRORED_REPEAT,
    MUF_WRAP_MODE_REPEAT,
    MUF_WRAP_MODE_MIRROR_CLAMP_TO_EDGE,
    MUF_ENUM_COUNT(MUF_WRAP_MODE)
} MufWrapMode;

typedef enum MufTextureFlags_e {
    MUF_TEXTURE_FLAGS_NONE          = 0x0000,
    MUF_TEXTURE_FLAGS_MULTISAMPLES  = 0x0001,
    MUF_TEXTURE_FLAGS_CUBE_MAP      = 0x0002,
    MUF_TEXTURE_FLAGS_STORAGE       = 0x0004,
} MufTextureFlags;

typedef struct MufTextureCreateInfo_s {
    MufTextureType  type;
    MufTextureFlags flags;
    MufAccessFlags  accessFlags;
    muf_u32         width;
    muf_u32         height;
    muf_u32         depth;
    muf_u32         arrayLayerCount;
    muf_usize       alignment;
    MufFormat       format;
    muf_u32         mipLevels;
    MufSampleCount  sampleCount;
} MufTextureCreateInfo;

MUF_HANDLE_DEF(MufTexture);

MUF_API MufTexture mufCreateTexture(const MufTextureCreateInfo *info);
MUF_API void mufDestroyTexture(MufTexture texture);

typedef struct MufSamplerCreateInfo_s {
    MufTextureFilter        minFilter;
    MufTextureFilter        magFilter;
    MufWrapMode             wrapModeU;
    MufWrapMode             wrapModeV;
    MufWrapMode             wrapModeW;
    muf_bool                mipMapEnabled;
    muf_u32                 baseMipLevel;
    muf_u32                 maxMipLevel;
    MufTextureFilter        mipFilter;
    muf_f32                 mipLODBias;
    muf_f32                 minLOD;
    muf_f32                 maxLOD;
    MufRGBA                 borderColor;
    MufTextureCompareMode   compareMode;
    MufCompareOperator      compareOp;
} MufSamplerCreateInfo;

MUF_HANDLE_DEF(MufSampler);

MufSampler mufCreateSampler(const MufSamplerCreateInfo *info);
void mufDestroySampler(MufSampler sampler);

typedef enum MufShaderType_e {
    MUF_ENUM_UNKNOWN(MUF_SHADER_TYPE),
    MUF_SHADER_TYPE_VERTEX,
    MUF_SHADER_TYPE_FRAGMENT,
    MUF_ENUM_COUNT(MUF_SHADER_TYPE)
} MufShaderType;

typedef enum MufShaderSourceType_e {
    MUF_ENUM_UNKNOWN(MUF_SHADER_SOURCE_TYPE),
    MUF_SHADER_SOURCE_TYPE_TEXT,
    MUF_SHADER_SOURCE_TYPE_BINARY,
    MUF_SHADER_SOURCE_TYPE_EXT = 0xFFFF
} MufShaderSourceType;

typedef struct MufShaderCreateInfo_s {
    MufShaderType       stageType;
    MufShaderSourceType sourceType;
    muf_crawptr         source;
    muf_usize           sourceSize;
} MufShaderCreateInfo;

#define MUF_DEFAULT_SHADER_CREATE_INFO (MufShaderCreateInfo) { \
    .stageType = _MUF_SHADER_TYPE_UNKNOWN_, \
    .sourceType = _MUF_SHADER_SOURCE_TYPE_UNKNOWN_, \
    .source = NULL, \
    .sourceSize = 0 \
}

MUF_HANDLE_DEF(MufShader);

MUF_API MufShader mufCreateShader(const MufShaderCreateInfo *info);
MUF_API void mufDestroyShader(MufShader shader);

typedef struct MufShaderProgramCreateInfo_s {
    muf_usize shaderCount;
    MufShader *shaders;
    muf_bool  autoDestroy;
} MufShaderProgramCreateInfo;

MUF_HANDLE_DEF(MufShaderProgram);

MUF_API MufShaderProgram mufCreateShaderProgram(const MufShaderProgramCreateInfo *info);
MUF_API void mufDestroyShaderProgram(MufShaderProgram program);

typedef enum MufUniformType_e {
    MUF_UNIFORM_TYPE_FLOAT,
    MUF_UNIFORM_TYPE_FLOAT2,
    MUF_UNIFORM_TYPE_FLOAT3,
    MUF_UNIFORM_TYPE_FLOAT4,

    MUF_UNIFORM_TYPE_INT,
    MUF_UNIFORM_TYPE_INT2,
    MUF_UNIFORM_TYPE_INT3,
    MUF_UNIFORM_TYPE_INT4,

    MUF_UNIFORM_TYPE_SAMPLER,

    MUF_UNIFORM_TYPE_VEC2,
    MUF_UNIFORM_TYPE_VEC3,
    MUF_UNIFORM_TYPE_VEC4,
    
    MUF_UNIFORM_TYPE_MAT2X2,
    MUF_UNIFORM_TYPE_MAT3X3,
    MUF_UNIFORM_TYPE_MAT4X4
} MufUniformType;

typedef MufHandle MufUniform;

typedef struct MufUniformCreateInfo_s {

} MufUniformCreateInfo;

typedef enum MufAttachmentLoadOperator_e {
    MUF_ENUM_UNKNOWN(MUF_ATTACHMENT_LOAD_OP),
    MUF_ATTACHMENT_LOAD_OP_DONT_CARE,
    MUF_ATTACHMENT_LOAD_OP_LOAD,
    MUF_ATTACHMENT_LOAD_OP_CLEAR,
    MUF_ENUM_COUNT(MUF_ATTACHMENT_LOAD_OP)
} MufAttachmentLoadOperator;

typedef enum MufAttachmentStoreOperator_e {
    MUF_ENUM_UNKNOWN(MUF_ATTACHMENT_STORE_OP),
    MUF_ATTACHMENT_STORE_OP_DONT_CARE,
    MUF_ATTACHMENT_STORE_OP_STORE,
    MUF_ENUM_COUNT(MUF_ATTACHMENT_STORE_OP)
} MufAttachmentStoreOperator;

typedef enum MufAttachmentType_e {
    MUF_ENUM_UNKNOWN(MUF_ATTACHMENT_TYPE),
    MUF_ATTACHMENT_TYPE_COLOR,
    MUF_ATTACHMENT_TYPE_DEPTH,
    MUF_ATTACHMENT_TYPE_STENCIL,
    MUF_ATTACHMENT_TYPE_DEPTH_STENCIL,
    MUF_ENUM_COUNT(MUF_ATTACHMENT_TYPE)
} MufAttachmentType;

typedef struct MufAttachmentDesc_s {
    MufAttachmentType   type;
    MufFormat           format;
    MufTexture          texture;
    MufAttachmentLoadOperator  loadOp;
    MufAttachmentStoreOperator storeOp;
    MufAttachmentLoadOperator stencilLoadOp;
    MufAttachmentStoreOperator stencilStoreOp;
} MufAttachmentDesc;

typedef struct MufFramebufferCreateInfo_s {
    muf_u32                 width;
    muf_u32                 height;
    muf_u32                 attachmentCount;
    const MufAttachmentDesc *attachments;
} MufFramebufferCreateInfo;

MUF_HANDLE_DEF(MufFramebuffer);

MufFramebuffer mufCreateFramebuffer(const MufFramebufferCreateInfo *info);
void mufDestroyFramebuffer(MufFramebuffer frameBuffer);

typedef struct MufRenderPassCreateInfo_s {
    MufAttachmentLoadOperator   loadOp;
    MufAttachmentStoreOperator  storeOp;
    MufAttachmentLoadOperator   stencilLoadOp;
    MufAttachmentStoreOperator  stencilStoreOp;
} MufRenderPassCreateInfo;

MUF_HANDLE_DEF(MufRenderPass);

MufRenderPass mufCreateRenderPass(const MufRenderPassCreateInfo *info);
void mufDestroyRenderPass(MufRenderPass renderPass);

typedef enum MufResourceType_e {
    MUF_ENUM_UNKNOWN(MUF_RESOURCE_TYPE),
    MUF_RESOURCE_TYPE_SAMPLER,
    MUF_RESOURCE_TYPE_TEXTURE,
    MUF_RESOURCE_TYPE_COMBINED_TEXTURE_SAMPLER,
    MUF_RESOURCE_TYPE_STORAGE_TEXTURE,
    MUF_RESOURCE_TYPE_STORAGE_BUFFER,
    MUF_RESOURCE_TYPE_UNIFOM_BUFFER,
    MUF_ENUM_COUNT(MUF_RESOURCE_TYPE)
} MufResourceType;

typedef struct MufResourceBindingDesc_s {
    muf_u32             bindingIndex;
    muf_u32             arraySize;   
    MufShaderStageFlags stageFlags;
    MufResourceType     resourceType;
    union {
        struct {
            MufSampler sampler;
            MufTexture texture;
        } combined;
        MufSampler  sampler;
        MufTexture  texture;
        MufBuffer   buffer;
    } resource;
} MufResourceBindingDesc;

typedef struct MufResourceHeapCreateInfo_s {
    muf_u32                 bindingCount;
    MufResourceBindingDesc  *bindings;
} MufResourceHeapCreateInfo;

MUF_HANDLE_DEF(MufResourceHeap);

MufResourceHeap mufCreateResourceHeap(const MufResourceHeapCreateInfo *info);
void mufDestroyResourceHeap(MufResourceHeap resourceHeap);

#endif