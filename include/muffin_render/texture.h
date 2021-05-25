#ifndef _MUFFIN_RENDER_TEXTURE_H_
#define _MUFFIN_RENDER_TEXTURE_H_

#include "muffin_core/common.h"
#include "muffin_render/format.h"

typedef enum MufTextureType_e {
    MUF_TEXTURE_TYPE_1D,
    MUF_TEXTURE_TYPE_2D,
    MUF_TEXTURE_TYPE_3D,
    MUF_TEXTURE_TYPE_1D_ARRAY,
    MUF_TEXTURE_TYPE_2D_ARRAY,
    MUF_TEXTURE_TYPE_CUBE_MAP,
    MUF_TEXTURE_TYPE_CUBE_MAP_ARRAY
} MufTextureType;

typedef enum MufTextureCompareMode_e {
    MUF_TEXTURE_COMPARE_MODE_NONE,
    MUF_TEXTURE_COMPARE_MODE_REF
} MufTextureCompareMode;

typedef enum MufTextureFilter_e {
    MUF_TEXTURE_FILTER_NEAREST,
    MUF_TEXTURE_FILTER_LINEAR,
    MUF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST,
    MUF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR,
    MUF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST,
    MUF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR,
} MufTextureFilter;

typedef enum MufWrapMode_e {
    MUF_WRAP_MODE_CLAMP_TO_EDGE,
    MUF_WRAP_MODE_CLAMP_TO_BORDER,
    MUF_WRAP_MODE_MIRRORED_REPEAT,
    MUF_WRAP_MODE_REPEAT,
    MUF_WRAP_MODE_MIRROR_CLAMP_TO_EDGE,
} MufWrapMode;

typedef struct MufSamplerCreateInfo_s {
    MufTextureFilter        minFilter;
    MufTextureFilter        magFilter;
    MufWrapMode             wrapModes[3];
    muf_u32                 baseMipLevel;
    muf_u32                 maxMipLevel;
    muf_f32                 mipLODBias;
    muf_f32                 minLOD;
    muf_f32                 maxLOD;
    MufColor                borderColor;
    MufTextureCompareMode   compareMode;
    MufCompareOperator      compareOp;
} MufSamplerCreateInfo;

typedef struct MufTextureCreateInfo_s {
    MufTextureType          textureType;
    MufFormat               format;
    muf_u32                 width;
    muf_u32                 height;
    muf_u32                 depthOrArraySize;
    const muf_rawptr        data;
    muf_u32                 mipLevel;
    muf_usize               alignment;
    MufSamplerCreateInfo    sampler;
    
} MufTextureCreateInfo;

typedef MufHandle MufTexture;

MufTexture mufCreateTexture(const MufTextureCreateInfo *info);
void mufDestroyTexture(MufTexture texture);


#endif