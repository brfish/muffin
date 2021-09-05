#include "muffin_render/backends/gl_backend.h"

#include "glad/glad.h"

#include "muffin_core/array.h"
#include "muffin_core/hash_map.h"
#include "muffin_core/log.h"
#include "muffin_core/memory.h"
#include "muffin_render/backend.h"
#include "muffin_render/commands.h"
#include "muffin_render/enums.h"
#include "muffin_render/pipeline.h"
#include "muffin_render/resources.h"

MUF_INTERNAL MufBuffer mufGLCreateBuffer(const MufBufferCreateInfo *info);
MUF_INTERNAL void mufGLDestroyBuffer(MufBuffer buffer);
MUF_INTERNAL MufSampler mufGLCreateSampler(const MufSamplerCreateInfo *info);
MUF_INTERNAL void mufGLDestroySampler(MufSampler sampler);
MUF_INTERNAL MufTexture mufGLCreateTexture(const MufTextureCreateInfo *info);
MUF_INTERNAL void mufGLDestroyTexture(MufTexture texture);
MUF_INTERNAL MufFramebuffer mufGLCreateFramebuffer(const MufFramebufferCreateInfo *info);
MUF_INTERNAL void mufGLDestroyFramebuffer(MufFramebuffer framebuffer);
MUF_INTERNAL MufShader mufGLCreateShader(const MufShaderCreateInfo *info);
MUF_INTERNAL void mufGLDestroyShader(MufShader shader);
MUF_INTERNAL MufShaderProgram mufGLCreateShaderProgram(const MufShaderProgramCreateInfo *info);
MUF_INTERNAL void mufGLDestroyShaderProgram(MufShaderProgram program);

MUF_INTERNAL MufResourceHeap mufGLCreateResourceHeap(const MufResourceHeapCreateInfo *info);
MUF_INTERNAL void mufGLDestroyResourceHeap(MufResourceHeap heap);

MUF_INTERNAL MufPipeline mufGLCreatePipeline(const MufPipelineCreateInfo *info);
MUF_INTERNAL void mufGLDestroyPipeline(MufPipeline pipeline);

MUF_INTERNAL void mufGLCmdCopyBuffer(MufBuffer dst, muf_offset dstOffset, MufBuffer src, muf_offset srcOffset, muf_usize size);
MUF_INTERNAL void mufGLCmdUpdateBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_crawptr data);
MUF_INTERNAL void mufGLCmdFillBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_u32 data);

MUF_INTERNAL void mufGLCmdCopyTexture(MufTexture dst, MufTextureCopyPos dstPos, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size);
MUF_INTERNAL void mufGLCmdCopyTextureToBuffer(MufBuffer dst, muf_usize dstOffset, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size);
MUF_INTERNAL void mufGLCmdCopyBufferToTexture(MufTexture dst, MufTextureCopyPos dstPos, MufBuffer src, muf_offset srcOffset, MufExtent3i size);
MUF_INTERNAL void mufGLCmdGenerateMipmaps(MufTexture texture);

MUF_INTERNAL void mufGLCmdSetPrimitiveTopology(MufPrimitiveTopology topology);
MUF_INTERNAL void mufGLCmdSetPrimitiveRestartEnabled(muf_bool enabled);
MUF_INTERNAL void mufGLCmdSetViewport(const MufViewport *viewport);
MUF_INTERNAL void mufGLCmdSetScissor(const MufScissor *scissor);
MUF_INTERNAL void mufGLCmdSetLineWidth(muf_f32 width);
MUF_INTERNAL void mufGLCmdSetStencilWriteMask(MufStencilFaceFlags face, muf_u32 mask);
MUF_INTERNAL void mufGLCmdSetStencilCompareMask(MufStencilFaceFlags face, muf_u32 mask);
MUF_INTERNAL void mufGLCmdSetStencilRef(MufStencilFaceFlags face, muf_u32 ref);
MUF_INTERNAL void mufGLCmdSetBlendConstant(MufRGBA rgba);
MUF_INTERNAL void mufGLCmdSetClearColor(MufRGBA rgba);
MUF_INTERNAL void mufGLCmdSetClearDepth(muf_f32 depth);
MUF_INTERNAL void mufGLCmdSetClearStencil(muf_u32 stencil);
MUF_INTERNAL void mufGLCmdBindPipeline(MufPipeline pipeline);
MUF_INTERNAL void mufGLCmdBindVertexBuffers(const MufBuffer *buffers, const muf_offset *offsets, muf_index firstBindingIndex, muf_usize bindingCount);
MUF_INTERNAL void mufGLCmdBindVertexBuffer(MufBuffer buffer, muf_offset offset, muf_index bindingIndex);
MUF_INTERNAL void mufGLCmdBindIndexBuffer(MufBuffer buffer, muf_offset offset);
MUF_INTERNAL void mufGLCmdBindResourceHeap(MufResourceHeap heap);

MUF_INTERNAL void mufGLCmdDraw(muf_index firstIndex, muf_usize count);
MUF_INTERNAL void mufGLCmdDrawIndexed(muf_index firstIndex, muf_usize count);
MUF_INTERNAL void mufGLCmdBlit();

/// GL backend global configuration

enum _MUFGL_GLOBAL_CONFIG {
    MUFGL_MAX_COLOR_ATTACHMENT = 16,
    MUFGL_MAX_DEPTH_ATTACHMENT = 16,
    MUFGL_MAX_SHADER_INFO_LEN = 512,
    MUFGL_MAX_SHADER_PROGRAM_INFO_LEN = 512,

    MUFGL_MAX_PIPELINE_COUNT = 32,

    MUFGL_PIPELINE_STATE_POOL_BUCKET_SIZE = 32,
    MUFGL_PIPELINE_STATE_POOL_INIT_BITS = MUF_U32_MAX
};

typedef struct _MufGLInputAssemblyState_s {
    GLenum      topology;
    GLboolean   primitiveRestartEnabled;
} _MufGLInputAssemblyState;

typedef struct _MufGLVertexInputAttribute_s {
    GLenum format;
    GLenum dataType;
    GLuint componentNum;
    GLuint location;
    GLvoid *offset;
} _MufGLVertexInputAttribute;

typedef struct _MufGLVertexInputLayout_s {
    GLuint                          attributeCount;
    _MufGLVertexInputAttribute      *attributes;
    GLsizei                         stride;
} _MufGLVertexInputLayout;

typedef struct _MufGLViewport_s {
    GLint   x;
    GLint   y;
    GLsizei width;
    GLsizei height;
    GLfloat minDepth;
    GLfloat maxDepth;
} _MufGLViewport;

typedef struct _MufGLScissor_s {
    GLint   x;
    GLint   y;
    GLsizei width;
    GLsizei height;
} _MufGLScissor;

typedef struct _MufGLDepthState_s {
    GLboolean   testEnabled;
    GLboolean   writeEnabled;
    GLenum      compareOp;
} _MufGLDepthState;

typedef struct _MufGLStencilOpState_s {
    GLenum failOp;
    GLenum passOp;
    GLenum depthFailOp;
    GLenum compareOp;
    GLuint writeMask;
    GLuint compareMask;
    GLuint ref;
} _MufGLStencilOpState;

typedef struct _MufGLStencilState_s {
    GLboolean               testEnabled;
    _MufGLStencilOpState    front;
    _MufGLStencilOpState    back;
} _MufGLStencilState;

typedef struct _MufGLBlendState_s {
    GLboolean   enabled;
    GLenum      srcColorFactor;
    GLenum      dstColorFactor;
    GLenum      colorBlendOp;
    GLenum      srcAlphaFactor;
    GLenum      dstAlphaFactor;
    GLenum      alphaBlendOp;
    GLfloat     constant[4];
} _MufGLBlendState;

typedef struct _MufGLRasterizerState_s {
    GLenum      cullMode;
    GLenum      frontFace;
    GLenum      polygonMode;
    GLfloat     lineWidth;
    GLboolean   depthClampEnabled;
    GLboolean   depthBiasEnabled;
    GLfloat     depthBiasConstantFactor;
    GLfloat     depthBiasClamp;
    GLfloat     depthBiasSlopFactor;
    GLboolean   multiSampleEnabled;
    GLuint      multiSampleCount;
} _MufGLRasterizerState;

typedef enum _MufGLCapabilityFlags_e {
    MUFGL_CAP_FLAGS_BLEND               = 1 << 0,
    MUFGL_CAP_FLAGS_CULL_FACE           = 1 << 1,
    MUFGL_CAP_FLAGS_DEPTH_CLAMP         = 1 << 2,
    MUFGL_CAP_FLAGS_DEPTH_TEST          = 1 << 3,
    MUFGL_CAP_FLAGS_FRAMEBUFFER_SRGB    = 1 << 4,
    MUFGL_CAP_FLAGS_MULTISAMPLE         = 1 << 5,
    MUFGL_CAP_FLAGS_SCISSOR_TEST        = 1 << 6,
    MUFGL_CAP_FLAGS_STENCIL_TEST        = 1 << 7,

} _MufGLCapabilityFlags;

typedef struct _MufGLBuffer_s {
    GLuint      resourceId;
    GLenum      target;
    GLbitfield  flags;
    GLsizeiptr  offset;
    GLsizeiptr  size;
    GLuint      alignment;
    GLbitfield  storageFlags;
    GLenum      access;
} _MufGLBuffer;

typedef struct _MufGLSampler_s {
    GLuint      resourceId;
    GLenum      compareMode;
    GLenum      compareOp;
} _MufGLSampler;

typedef struct _MufGLTexture_s {
    GLuint      resourceId;
    GLenum      target;
    GLbitfield  flags;
    GLenum      access;
    GLuint      width;
    GLuint      height;
    GLuint      depth;
    GLuint      arrayLayerCount;
    GLenum      internalFormat;
    GLenum      format;
    GLenum      pixelType;
    GLuint      mipLevels;
    GLuint      sampleCount;
    GLuint      samplerId;
} _MufGLTexture;

typedef struct _MufGLFramebuffer_s {
    GLuint resourceId;
    GLenum target;
    GLbitfield flags;
    GLuint colorAttachments[MUFGL_MAX_COLOR_ATTACHMENT];
    union {
        GLuint depthStencilAttachment;
        struct {
            GLuint depthAttachment;
            GLuint stencilAttachment;
        };
    };
} _MufGLFramebuffer;

typedef struct _MufGLShader_s {
    GLuint      resourceId;
    GLboolean   compiled;
    GLchar      compileInfo[MUFGL_MAX_SHADER_INFO_LEN];
} _MufGLShader;

typedef struct _MufGLShaderProgram_s {
    GLuint      resourceId;
    GLboolean   linked;
    GLchar      linkInfo[MUFGL_MAX_SHADER_PROGRAM_INFO_LEN];
} _MufGLShaderProgram;

typedef struct _MufGLResourceBindingDesc_s {
    MufResourceType resourceType;
    muf_rawptr      resourcePtr0;
    muf_rawptr      resourcePtr1;
    GLuint          bindingIndex;
    GLuint          arraySize;
} _MufGLResourceBindingDesc;

typedef struct _MufGLResourceHeap_s {
    GLuint                      bindingCount;
    _MufGLResourceBindingDesc   *bindings;
} _MufGLResourceHeap;

typedef struct _MufGLVertexArray_s {
    GLuint          resourceId;
    GLsizei         baseOffset;
    GLsizei         stride;
    GLuint          bindingIndex;
    _MufGLBuffer    *indexBuffer;
} _MufGLVertexArray;

typedef struct _MufGLRenderPass_s {
    GLuint      resourceId;
    GLbitfield  clearBits;
    GLuint      sampleCount;
} _MufGLRenderPass;

typedef struct _MufGLQuery_s {

} _MufGLQuery;

typedef struct _MufGLPipelineState_s {
    GLuint                      id;
    GLuint                      program;
    _MufGLVertexArray           vertexArray;
    _MufGLInputAssemblyState    inputAssembly;
    _MufGLVertexInputLayout     inputLayout;
    _MufGLViewport              viewport;
    _MufGLScissor               scissor;
    _MufGLDepthState            depth;
    _MufGLStencilState          stencil;
    _MufGLBlendState            blend;
    _MufGLRasterizerState       rasterizer;
} _MufGLPipelineState;

const _MufGLPipelineState _defaultPipelineState = {
    .id = (GLuint) -1,
    .program = 0,
    .vertexArray = { 0 },
    .inputAssembly = {
        .topology = GL_TRIANGLES,
        .primitiveRestartEnabled = MUF_TRUE,
    },
    .inputLayout = { 0 },
    .viewport = { 0 },
    .scissor = { 0 },
    .depth = { 
        .testEnabled = GL_FALSE,
        .writeEnabled = GL_FALSE,
        .compareOp = GL_LESS
    },
    .stencil = {
        .testEnabled = GL_FALSE,
        .front = {
            .failOp = GL_KEEP,
            .passOp = GL_KEEP,
            .depthFailOp = GL_KEEP,
            .compareOp = GL_ALWAYS,
            .writeMask = 0xFFFFFFFF,
            .compareMask = 0xFFFFFFFF,
            .ref = 0
        },
        .back = {
            .failOp = GL_KEEP,
            .passOp = GL_KEEP,
            .depthFailOp = GL_KEEP,
            .compareOp = GL_ALWAYS,
            .writeMask = 0xFFFFFFFF,
            .compareMask = 0xFFFFFFFF,
            .ref = 0
        }
    },
    .blend = {
        .enabled = GL_FALSE,
        .srcColorFactor = GL_ONE,
        .dstColorFactor = GL_ZERO,
        .colorBlendOp = GL_FUNC_ADD,
        .srcAlphaFactor = GL_ONE,
        .dstAlphaFactor = GL_ZERO,
        .alphaBlendOp = GL_FUNC_ADD,
        .constant = { 0.0F, 0.0F, 0.0F, 0.0F }
    },
    .rasterizer = {
        .cullMode = GL_BACK,
        .frontFace = GL_CCW,
        .polygonMode = GL_FILL,
        .lineWidth = 1.0F,
        .depthBiasEnabled = GL_FALSE,
        .multiSampleEnabled = GL_FALSE,
        .multiSampleCount = 1
    }
};

typedef struct _MufGLObjectPool_s {
    MufArray *objects;
    MufArray *bitset;
} _MufGLObjectPool;

enum {
    MUFGL_OBJECT_POOL_INIT_COUNT = 32,
    MUFGL_OBJECT_POOL_INIT_BITS = MUF_U32_MAX
};

static MUF_INLINE _MufGLObjectPool *_mufGLCreateObjectPool(muf_usize objectSize, muf_rawptr initContent) {
    _MufGLObjectPool *pool = mufAlloc(_MufGLObjectPool, 1);
    pool->objects = _mufCreateArray(objectSize);
    pool->bitset = mufCreateArray(muf_u32);
    mufArrayResize(pool->objects, MUFGL_OBJECT_POOL_INIT_COUNT, initContent);
    muf_u32 initBits = MUFGL_OBJECT_POOL_INIT_BITS;
    mufArrayPush(pool->bitset, &initBits);
    return pool;
}

static MUF_INLINE void _mufGLDestroyObjectPool(_MufGLObjectPool *pool) {
    mufDestroyArray(pool->objects);
    mufDestroyArray(pool->bitset);
    mufFree(pool);
}

static muf_rawptr _mufGLObjectPoolRequire(_MufGLObjectPool *pool, muf_rawptr initContent, muf_index *index) {
    muf_usize bucketCount = mufArrayGetSize(pool->bitset);
    for (muf_index i = 0; i < bucketCount; ++i) {
        muf_u32 *bits = (muf_u32 *) mufArrayGetRef(pool->bitset, i);
        if (*bits != 0) {
            muf_u32 firstUsableIndex = mufBitsFindFirstSet(*bits);
            mufBitsReset(bits, firstUsableIndex);
            firstUsableIndex += 32 * i;
            muf_rawptr obj = mufArrayGetRef(pool->objects, firstUsableIndex);
            mufMemCopyBytes(obj, initContent, pool->objects->elementSize);
            *index = firstUsableIndex;
            return obj;
        }
    }
    muf_u32 newBits = MUFGL_OBJECT_POOL_INIT_BITS & (~1);
    mufArrayPush(pool->bitset, &newBits);
    mufArrayResize(pool->objects, pool->objects->size + 32, &_defaultPipelineState);
    muf_rawptr obj = mufArrayGetFrontRef(pool->objects);
    *index = pool->objects->size - 31;
    return obj;
}

static MUF_INLINE void _mufGLObjectPoolRecycle(_MufGLObjectPool *pool, muf_u32 index) {
    muf_index bucketIndex = index / 32;
    muf_index bitIndex = index % 32;
    
    muf_rawptr *bitsRef = mufArrayGetRef(pool->bitset, bucketIndex);
    *((muf_u32 *) bitsRef) |= 1 << bitIndex;
}

static MUF_INLINE _MufGLPipelineState *_mufGLObjectPoolGet(_MufGLObjectPool *pool, muf_u32 index) {
    return mufArrayGetRef(pool->objects, index);
}

typedef struct _MufGLPipelineStatePool_s {
    MufArray *pipelines;
    MufArray *bitset;
} _MufGLPipelineStatePool;

static MUF_INLINE  _MufGLPipelineStatePool *_mufGLCreatePipelineStatePool() {
    _MufGLPipelineStatePool *pool = mufAlloc(_MufGLPipelineStatePool, 1);
    pool->pipelines = mufCreateArray(_MufGLPipelineState);
    pool->bitset = mufCreateArray(muf_u32);
    mufArrayResize(pool->pipelines, MUFGL_PIPELINE_STATE_POOL_BUCKET_SIZE, &_defaultPipelineState);
    muf_u32 first = MUFGL_PIPELINE_STATE_POOL_INIT_BITS;
    mufArrayPush(pool->bitset, &first);
    return pool;
}

static MUF_INLINE void _mufGLDestroyPipelineStatePool(_MufGLPipelineStatePool *pool) {
    mufDestroyArray(pool->pipelines);
    mufDestroyArray(pool->bitset);
    mufFree(pool);
}

static _MufGLPipelineState *_mufGLPipelineStatePoolRequire(_MufGLPipelineStatePool *pool) {
    muf_usize bucketCount = mufArrayGetSize(pool->bitset);
    for (muf_index i = 0; i < bucketCount; ++i) {
        muf_u32 *bits = (muf_u32 *) mufArrayGetRef(pool->bitset, i);
        if (*bits != 0) {
            muf_u32 firstUsableIndex = mufBitsFindFirstSet(*bits);
            mufBitsReset(bits, firstUsableIndex);
            firstUsableIndex += 32 * i;
            _MufGLPipelineState *state = mufArrayGetRef(pool->pipelines, firstUsableIndex);
            mufMemCopy(state, &_defaultPipelineState, _MufGLPipelineState, 1);
            state->id = firstUsableIndex;
            return state;
        }
    }
    muf_u32 newBits = MUFGL_PIPELINE_STATE_POOL_INIT_BITS & (~1);
    mufArrayPush(pool->bitset, &newBits);
    mufArrayResize(pool->pipelines, pool->pipelines->size + 32, &_defaultPipelineState);
    _MufGLPipelineState *state = (_MufGLPipelineState *) mufArrayGetFrontRef(pool->pipelines);
    state->id = pool->pipelines->size - 31;
    return state;
}

static MUF_INLINE void _mufGLPipelineStatePoolRecycle(_MufGLPipelineStatePool *pool, muf_u32 pipelineId) {
    muf_index bucketIndex = pipelineId / 32;
    muf_index bitIndex = pipelineId % 32;
    
    muf_rawptr *bitsRef = mufArrayGetRef(pool->bitset, bucketIndex);
    *((muf_u32 *) bitsRef) |= 1 << bitIndex;
}

static MUF_INLINE _MufGLPipelineState *_mufGLPipelineStatePoolGet(_MufGLPipelineStatePool *pool, muf_u32 pipelineId) {
    return mufArrayGetRef(pool->pipelines, pipelineId);
}

typedef struct _MufGLConfig_s {
    const GLubyte *vendorName;
    const GLubyte *rendererName;
    const GLubyte *version;
    const GLubyte *glslVersion;

    GLint majorVersion;
    GLint minorVersion;

    GLint maxFrameBufferWidth;
    GLint maxFrameBufferHeight;
    GLint maxTextureBufferSize;
    
    GLint extensionCount;
    const GLubyte **extensions;

} _MufGLConfig;
    
_MufGLConfig _glConfig;

static void _mufGLinitConfig() {
    _MufGLConfig *c = &_glConfig;
    c->vendorName = glGetString(GL_VENDOR);
    c->rendererName = glGetString(GL_RENDERER);
    c->version = glGetString(GL_VERSION);
    c->glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    glGetIntegerv(GL_MAJOR_VERSION, &c->majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &c->minorVersion);
    
    glGetIntegerv(GL_NUM_EXTENSIONS, &c->extensionCount);
    c->extensions = mufAlloc(const muf_uchar *, c->extensionCount);
    for (GLuint i = 0; i < c->extensionCount; ++i) {
        c->extensions[i] = glGetStringi(GL_EXTENSIONS, i);
    }

    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &c->maxFrameBufferWidth);
    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &c->maxFrameBufferHeight);

}

typedef struct _MufGLCache_s {
    struct _Pools {
        _MufGLPipelineStatePool *pipelinePool;
    } pools;
    _MufGLPipelineState *pipeline;
    _MufGLRenderPass    *renderPass;
    _MufGLResourceHeap  *resourceHeap;
} _MufGLCache;

_MufGLCache _mufGLCache[1];

static void _mufGLInitCache() {
    _mufGLCache->pools.pipelinePool = _mufGLCreatePipelineStatePool();
    _MufGLPipelineState *defaultState = _mufGLPipelineStatePoolRequire(_mufGLCache->pools.pipelinePool);
    _mufGLCache->pipeline = defaultState;
    _mufGLCache->renderPass = NULL;
}

static void _mufGLFinishCache() {
    _mufGLDestroyPipelineStatePool(_mufGLCache->pools.pipelinePool);
}

static GLenum _mufGLConvertFormat(MufFormat format) {
    switch (format) {
        case MUF_FORMAT_R8_UINT             : return GL_R8UI;
        case MUF_FORMAT_R8_SINT             : return GL_R8I;
        case MUF_FORMAT_R8_UNORM            : return GL_R8;
        case MUF_FORMAT_R8_SNORM            : return GL_R8_SNORM;
        case MUF_FORMAT_R8G8_UINT           : return GL_RG8UI;
        case MUF_FORMAT_R8G8_SINT           : return GL_RG8I;
        case MUF_FORMAT_R8G8_UNORM          : return GL_RG8;
        case MUF_FORMAT_R8G8_SNORM          : return GL_RG8_SNORM;
        case MUF_FORMAT_R8G8B8_UINT         : return GL_RGB8UI;
        case MUF_FORMAT_R8G8B8_SINT         : return GL_RGB8I;
        case MUF_FORMAT_R8G8B8_UNORM        : return GL_RGB8;
        case MUF_FORMAT_R8G8B8_SNORM        : return GL_RGB8_SNORM;
        case MUF_FORMAT_R8G8B8A8_UINT       : return GL_RGBA8UI;
        case MUF_FORMAT_R8G8B8A8_SINT       : return GL_RGBA8I;
        case MUF_FORMAT_R8G8B8A8_UNORM      : return GL_RGBA8;
        case MUF_FORMAT_R8G8B8A8_SNORM      : return GL_RGBA8_SNORM;

        case MUF_FORMAT_R16_UINT            : return GL_R16UI;
        case MUF_FORMAT_R16_SINT            : return GL_R16I;
        case MUF_FORMAT_R16_UNORM           : return GL_R16;
        case MUF_FORMAT_R16_SNORM           : return GL_R16_SNORM;
        case MUF_FORMAT_R16G16_UINT         : return GL_RG16UI;
        case MUF_FORMAT_R16G16_SINT         : return GL_RG16I;
        case MUF_FORMAT_R16G16_UNORM        : return GL_RG16;
        case MUF_FORMAT_R16G16_SNORM        : return GL_RG16_SNORM;
        case MUF_FORMAT_R16G16B16_UINT      : return GL_RGB16UI;
        case MUF_FORMAT_R16G16B16_SINT      : return GL_RGB16I;
        case MUF_FORMAT_R16G16B16_UNORM     : return GL_RGB16;
        case MUF_FORMAT_R16G16B16_SNORM     : return GL_RGB16_SNORM;
        case MUF_FORMAT_R16G16B16A16_UINT   : return GL_RGBA16UI;
        case MUF_FORMAT_R16G16B16A16_SINT   : return GL_RGBA16I;
        case MUF_FORMAT_R16G16B16A16_UNORM  : return GL_RGBA16;
        case MUF_FORMAT_R16G16B16A16_SNORM  : return GL_RGBA16_SNORM;

        case MUF_FORMAT_R32_UINT            : return GL_R32UI;
        case MUF_FORMAT_R32_SINT            : return GL_R32I;
        case MUF_FORMAT_R32_FLOAT           : return GL_R32F;
        case MUF_FORMAT_R32G32_UINT         : return GL_RG32UI;
        case MUF_FORMAT_R32G32_SINT         : return GL_RG32I;
        case MUF_FORMAT_R32G32_FLOAT        : return GL_RG32F;
        case MUF_FORMAT_R32G32B32_UINT      : return GL_RGB32UI;
        case MUF_FORMAT_R32G32B32_SINT      : return GL_RGB32I;
        case MUF_FORMAT_R32G32B32_FLOAT     : return GL_RGB32F;
        case MUF_FORMAT_R32G32B32A32_UINT   : return GL_RGBA32UI;
        case MUF_FORMAT_R32G32B32A32_SINT   : return GL_RGBA32I;
        case MUF_FORMAT_R32G32B32A32_FLOAT  : return GL_RGBA32F;

        case MUF_FORMAT_D16_UNORM           : return GL_DEPTH_COMPONENT16;
        case MUF_FORMAT_D32_FLOAT           : return GL_DEPTH_COMPONENT32F;
        case MUF_FORMAT_D24_UNORM_S8_UINT   : return GL_DEPTH24_STENCIL8;
        case MUF_FORMAT_D32_FLOAT_S8_UINT   : return GL_DEPTH32F_STENCIL8;

        default                             : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertPixelDataFormat(MufFormat format) {
    switch (format) {
        case MUF_FORMAT_R8_UINT             :
        case MUF_FORMAT_R8_SINT             :
        case MUF_FORMAT_R8_UNORM            :
        case MUF_FORMAT_R8_SNORM            :
        case MUF_FORMAT_R16_UINT            :
        case MUF_FORMAT_R16_SINT            :
        case MUF_FORMAT_R16_UNORM           :
        case MUF_FORMAT_R16_SNORM           :
        case MUF_FORMAT_R32_UINT            :
        case MUF_FORMAT_R32_SINT            : return GL_RED_INTEGER;

        case MUF_FORMAT_R8G8_UINT           :
        case MUF_FORMAT_R8G8_SINT           :
        case MUF_FORMAT_R8G8_UNORM          :
        case MUF_FORMAT_R8G8_SNORM          :
        case MUF_FORMAT_R16G16_UINT         :
        case MUF_FORMAT_R16G16_SINT         :
        case MUF_FORMAT_R16G16_UNORM        :
        case MUF_FORMAT_R16G16_SNORM        :
        case MUF_FORMAT_R32G32_UINT         :
        case MUF_FORMAT_R32G32_SINT         : return GL_RG_INTEGER;
        
        case MUF_FORMAT_R8G8B8_UINT         :
        case MUF_FORMAT_R8G8B8_SINT         :
        case MUF_FORMAT_R8G8B8_UNORM        :
        case MUF_FORMAT_R8G8B8_SNORM        :
        case MUF_FORMAT_R16G16B16_UINT      :
        case MUF_FORMAT_R16G16B16_SINT      :
        case MUF_FORMAT_R16G16B16_UNORM     :
        case MUF_FORMAT_R16G16B16_SNORM     :
        case MUF_FORMAT_R32G32B32_UINT      :
        case MUF_FORMAT_R32G32B32_SINT      : return GL_RGB_INTEGER;
        
        case MUF_FORMAT_R8G8B8A8_UINT       :
        case MUF_FORMAT_R8G8B8A8_SINT       :
        case MUF_FORMAT_R8G8B8A8_UNORM      :
        case MUF_FORMAT_R8G8B8A8_SNORM      :
        case MUF_FORMAT_R16G16B16A16_UINT   :
        case MUF_FORMAT_R16G16B16A16_SINT   :
        case MUF_FORMAT_R16G16B16A16_UNORM  :
        case MUF_FORMAT_R16G16B16A16_SNORM  :
        case MUF_FORMAT_R32G32B32A32_UINT   :
        case MUF_FORMAT_R32G32B32A32_SINT   : return GL_RGBA_INTEGER;

        
        case MUF_FORMAT_R32_FLOAT           : return GL_RED;
        
        case MUF_FORMAT_R32G32_FLOAT        : return GL_RG;
        
        case MUF_FORMAT_R32G32B32_FLOAT     : return GL_RGB;
        
        case MUF_FORMAT_R32G32B32A32_FLOAT  : return GL_RGBA;

        case MUF_FORMAT_D16_UNORM           :
        case MUF_FORMAT_D32_FLOAT           : return GL_DEPTH_COMPONENT;

        case MUF_FORMAT_D24_UNORM_S8_UINT   :
        case MUF_FORMAT_D32_FLOAT_S8_UINT   : return GL_DEPTH_STENCIL;

        default                             : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertPixelDataType(MufFormat format) {
    switch (format) {
        case MUF_FORMAT_R8_UINT             :
        case MUF_FORMAT_R8_UNORM            :
        case MUF_FORMAT_R8G8_UINT           :
        case MUF_FORMAT_R8G8_UNORM          :
        case MUF_FORMAT_R8G8B8_UINT         :
        case MUF_FORMAT_R8G8B8_UNORM        :
        case MUF_FORMAT_R8G8B8A8_UINT       :
        case MUF_FORMAT_R8G8B8A8_UNORM      : return GL_UNSIGNED_BYTE;

        case MUF_FORMAT_R16_UINT            :
        case MUF_FORMAT_R16_UNORM           :
        case MUF_FORMAT_R16G16_UINT         :
        case MUF_FORMAT_R16G16_UNORM        :
        case MUF_FORMAT_R16G16B16_UINT      :
        case MUF_FORMAT_R16G16B16_UNORM     :
        case MUF_FORMAT_R16G16B16A16_UINT   :
        case MUF_FORMAT_R16G16B16A16_UNORM  :
        case MUF_FORMAT_D16_UNORM           : return GL_UNSIGNED_SHORT;

        case MUF_FORMAT_R32_UINT            :
        case MUF_FORMAT_R32G32_UINT         :
        case MUF_FORMAT_R32G32B32_UINT      :
        case MUF_FORMAT_R32G32B32A32_UINT   : return GL_UNSIGNED_INT;

        case MUF_FORMAT_R8_SINT             :
        case MUF_FORMAT_R8_SNORM            :
        case MUF_FORMAT_R8G8_SINT           :
        case MUF_FORMAT_R8G8_SNORM          :
        case MUF_FORMAT_R8G8B8_SINT         :
        case MUF_FORMAT_R8G8B8_SNORM        :
        case MUF_FORMAT_R8G8B8A8_SINT       :
        case MUF_FORMAT_R8G8B8A8_SNORM      : return GL_BYTE;

        case MUF_FORMAT_R16_SINT            :
        case MUF_FORMAT_R16_SNORM           :
        case MUF_FORMAT_R16G16_SINT         :
        case MUF_FORMAT_R16G16_SNORM        :
        case MUF_FORMAT_R16G16B16_SINT      :
        case MUF_FORMAT_R16G16B16_SNORM     :
        case MUF_FORMAT_R16G16B16A16_SINT   :
        case MUF_FORMAT_R16G16B16A16_SNORM  : return GL_SHORT;
        
        case MUF_FORMAT_R32_SINT            :
        case MUF_FORMAT_R32G32_SINT         :
        case MUF_FORMAT_R32G32B32_SINT      :
        case MUF_FORMAT_R32G32B32A32_SINT   : return GL_INT;

        case MUF_FORMAT_R32_FLOAT           :
        case MUF_FORMAT_R32G32_FLOAT        :
        case MUF_FORMAT_R32G32B32_FLOAT     :
        case MUF_FORMAT_R32G32B32A32_FLOAT  :
        case MUF_FORMAT_D32_FLOAT           : return GL_FLOAT;

        case MUF_FORMAT_D24_UNORM_S8_UINT   : return GL_UNSIGNED_INT_24_8;

        case MUF_FORMAT_D32_FLOAT_S8_UINT   : return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

        default                             : MUF_UNREACHABLE();
    }
    return -1;
}

static GLuint _mufGLGetFormatSize(MufFormat format) {
    GLenum pixelDataFormat = _mufGLConvertPixelDataFormat(format);
    switch (pixelDataFormat) {
        case GL_RED             :
        case GL_RED_INTEGER     : return 1;
        case GL_RG              :
        case GL_RG_INTEGER      : return 2;
        case GL_RGB             :
        case GL_RGB_INTEGER     : return 3;
        case GL_RGBA            : 
        case GL_RGBA_INTEGER    : return 4;
        default                 : MUF_UNREACHABLE();
    }
    return -1;
}

static GLboolean _mufGLIsNormalized(MufFormat format) {
    switch (format) {
        case MUF_FORMAT_R8_UNORM            :
        case MUF_FORMAT_R8G8_UNORM          :
        case MUF_FORMAT_R8G8B8_UNORM        :
        case MUF_FORMAT_R8G8B8A8_UNORM      :
        case MUF_FORMAT_R16_UNORM           :
        case MUF_FORMAT_R16G16_UNORM        :
        case MUF_FORMAT_R16G16B16_UNORM     :
        case MUF_FORMAT_R16G16B16A16_UNORM  :
        case MUF_FORMAT_D16_UNORM           :
        case MUF_FORMAT_R8_SNORM            :
        case MUF_FORMAT_R8G8_SNORM          :
        case MUF_FORMAT_R8G8B8_SNORM        :
        case MUF_FORMAT_R8G8B8A8_SNORM      :
        case MUF_FORMAT_R16_SNORM           :
        case MUF_FORMAT_R16G16_SNORM        :
        case MUF_FORMAT_R16G16B16_SNORM     :
        case MUF_FORMAT_R16G16B16A16_SNORM  : return GL_TRUE;
        default                             : return GL_FALSE;
    }
}

static GLenum _mufGLConvertCompareOperator(MufCompareOperator op) {
    switch (op) {
        case MUF_COMPARE_OP_ALWAYS      : return GL_ALWAYS;
        case MUF_COMPARE_OP_NEVER       : return GL_NEVER;
        case MUF_COMPARE_OP_LESS        : return GL_LESS;
        case MUF_COMPARE_OP_GREATER     : return GL_GREATER;
        case MUF_COMPARE_OP_EQUAL       : return GL_EQUAL;
        case MUF_COMPARE_OP_NEQUAL      : return GL_NOTEQUAL;
        case MUF_COMPARE_OP_LEQAUL      : return GL_LEQUAL;
        case MUF_COMPARE_OP_GEQUAL      : return GL_GEQUAL;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertBufferType(MufBufferType type) {
    switch (type) {
        case MUF_BUFFER_TYPE_VERTEX     : return GL_ARRAY_BUFFER;
        case MUF_BUFFER_TYPE_INDEX      : return GL_ELEMENT_ARRAY_BUFFER;
        case MUF_BUFFER_TYPE_UNIFORM    : return GL_UNIFORM_BUFFER;
        case MUF_BUFFER_TYPE_STORAGE    : return GL_SHADER_STORAGE_BUFFER;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertAccessFlags(MufAccessFlags flags) {
    GLboolean read = flags & MUF_ACCESS_FLAGS_READ;
    GLboolean write = flags & MUF_ACCESS_FLAGS_WRITE;
    return read && write ? GL_READ_WRITE :
        (read ? GL_READ_ONLY : (write ? GL_WRITE_ONLY : GL_NONE));
}

static GLsizei _mufGLConvertSampleCount(MufSampleCount sampleCount) {
    switch (sampleCount) {
        case MUF_SAMPLE_COUNT_1     : return 1;
        case MUF_SAMPLE_COUNT_2     : return 2;
        case MUF_SAMPLE_COUNT_4     : return 4;
        case MUF_SAMPLE_COUNT_8     : return 8;
        case MUF_SAMPLE_COUNT_16    : return 16;
        default                     : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertTextureType(MufTextureType type, muf_bool isMultiSample, muf_bool isArray) {
    GLenum glType = -1;
    switch (type) {
        case MUF_TEXTURE_TYPE_1D : glType = GL_TEXTURE_1D; break;
        case MUF_TEXTURE_TYPE_2D : glType = GL_TEXTURE_2D; break;
        case MUF_TEXTURE_TYPE_3D : glType = GL_TEXTURE_3D; break;
    }
    if (isMultiSample) {
        if (glType == GL_TEXTURE_2D) {
            glType = GL_TEXTURE_2D_MULTISAMPLE;
        } else {
            MUF_UNREACHABLE();
        }
    }
    if (isArray) {
        switch (glType) {
            case GL_TEXTURE_1D              : glType = GL_TEXTURE_1D_ARRAY; break;
            case GL_TEXTURE_2D              : glType = GL_TEXTURE_2D_ARRAY; break;
            case GL_TEXTURE_2D_MULTISAMPLE  : glType = GL_TEXTURE_2D_MULTISAMPLE_ARRAY; break;
            default                         : MUF_UNREACHABLE();
        }
    }
    return glType;
}

static GLenum _mufGLConvertTextureFilter(MufTextureFilter filter) {
    switch (filter) {
        case MUF_TEXTURE_FILTER_NEAREST : return GL_NEAREST;
        case MUF_TEXTURE_FILTER_LINEAR  : return GL_LINEAR;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertMipmapTextureFilter(GLenum filter, GLenum mipFilter) {
    switch (filter) {
        case GL_NEAREST : return mipFilter == GL_NEAREST ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
        case GL_LINEAR  : return mipFilter == GL_NEAREST ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;
        default         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertWrapMode(MufWrapMode mode) {
    switch (mode) {
        case MUF_WRAP_MODE_CLAMP_TO_EDGE           : return GL_CLAMP_TO_EDGE;
        case MUF_WRAP_MODE_CLAMP_TO_BORDER         : return GL_CLAMP_TO_BORDER;
        case MUF_WRAP_MODE_MIRRORED_REPEAT         : return GL_MIRRORED_REPEAT;
        case MUF_WRAP_MODE_REPEAT                  : return GL_REPEAT;
        case MUF_WRAP_MODE_MIRROR_CLAMP_TO_EDGE    : return GL_MIRROR_CLAMP_TO_EDGE;
        default                                    : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertTextureCompareMode(MufTextureCompareMode mode) {
    switch (mode) {
        case MUF_TEXTURE_COMPARE_MODE_NONE  : return GL_NONE;
        case MUF_TEXTURE_COMPARE_MODE_REF   : return GL_COMPARE_REF_TO_TEXTURE;
        default                             : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertShaderType(MufShaderType type) {
    switch (type) {
        case MUF_SHADER_TYPE_VERTEX    : return GL_VERTEX_SHADER;
        case MUF_SHADER_TYPE_FRAGMENT  : return GL_FRAGMENT_SHADER;
        default                        : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertCullMode(MufCullModeFlags mode) {
    muf_bool back = mode & MUF_CULL_MODE_FLAGS_BACK;
    muf_bool front = mode & MUF_CULL_MODE_FLAGS_FRONT;

    if (back && front)
        return GL_FRONT_AND_BACK;
    if (back)
        return GL_BACK;
    if (front)
        return GL_BACK;
    MUF_UNREACHABLE();
    return -1;
}

static GLenum _mufGLConvertFrontFace(MufFrontFace mode) {
    switch (mode) {
        case MUF_FRONT_FACE_CW          : return GL_CW;
        case MUF_FRONT_FACE_CCW         : return GL_CCW;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertPolygonMode(MufPolygonMode mode) {
    switch (mode) {
        case MUF_POLYGON_MODE_POINT     : return GL_POINT;
        case MUF_POLYGON_MODE_LINE      : return GL_LINE;
        case MUF_POLYGON_MODE_FILL      : return GL_FILL;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertPrimitiveTopology(MufPrimitiveTopology primitive) {
    switch (primitive) {
        case MUF_PRIMITIVE_TOPOLOGY_POINTS          : return GL_POINTS;
        case MUF_PRIMITIVE_TOPOLOGY_LINES           : return GL_LINES;
        case MUF_PRIMITIVE_TOPOLOGY_LINE_STRIP      : return GL_LINE_STRIP;
        case MUF_PRIMITIVE_TOPOLOGY_TRIANGLES       : return GL_TRIANGLES;
        case MUF_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP  : return GL_TRIANGLE_STRIP;
        default                                 : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertStencilOperator(MufStencilOperator op) {
    switch (op) {
        case MUF_STENCIL_OP_KEEP        : return GL_KEEP;
        case MUF_STENCIL_OP_ZERO        : return GL_ZERO;
        case MUF_STENCIL_OP_REPLACE     : return GL_REPLACE;
        case MUF_STENCIL_OP_INC         : return GL_INCR;
        case MUF_STENCIL_OP_WARP_INC    : return GL_INCR_WRAP;
        case MUF_STENCIL_OP_DEC         : return GL_DECR;
        case MUF_STENCIL_OP_WRAP_DEC    : return GL_DECR_WRAP;
        case MUF_STENCIL_OP_INVERT      : return GL_INVERT;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertBlendOperator(MufBlendOperator op) {
    switch (op) {
        case MUF_BLEND_OP_ADD           : return GL_FUNC_ADD;
        case MUF_BLEND_OP_SUB           : return GL_FUNC_SUBTRACT;
        case MUF_BLEND_OP_REVERSE_SUB   : return GL_FUNC_REVERSE_SUBTRACT;
        default                         : MUF_UNREACHABLE();
    }
    return -1;
}

static GLenum _mufGLConvertBlendFactor(MufBlendFactor factor) {
    switch (factor) {
        case MUF_BLEND_FACTOR_ZERO                      : return GL_ZERO;
        case MUF_BLEND_FACTOR_ONE                       : return GL_ONE;
        case MUF_BLEND_FACTOR_SRC_COLOR                 : return GL_SRC_COLOR;
        case MUF_BLEND_FACTOR_DST_COLOR                 : return GL_DST_COLOR;
        case MUF_BLEND_FACTOR_ONE_MINUS_SRC_COLOR       : return GL_ONE_MINUS_SRC_COLOR;
        case MUF_BLEND_FACTOR_ONE_MINUS_DST_COLOR       : return GL_ONE_MINUS_DST_COLOR;
        case MUF_BLEND_FACTOR_SRC_ALPHA                 : return GL_SRC_ALPHA;
        case MUF_BLEND_FACTOR_DST_ALPHA                 : return GL_DST_ALPHA;
        case MUF_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA       : return GL_ONE_MINUS_SRC_ALPHA;
        case MUF_BLEND_FACTOR_ONE_MINUS_DST_ALPHA       : return GL_ONE_MINUS_DST_ALPHA;
        case MUF_BLEND_FACTOR_CONSTANT                  : return GL_CONSTANT_COLOR;
        case MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT        : return GL_ONE_MINUS_CONSTANT_COLOR;
        case MUF_BLEND_FACTOR_CONSTANT_ALPHA            : return GL_CONSTANT_ALPHA;
        case MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA  : return GL_ONE_MINUS_CONSTANT_ALPHA;
        default                                         : MUF_UNREACHABLE();
    }
    return -1;
}

MufBuffer mufGLCreateBuffer(const MufBufferCreateInfo *info) {
    GLuint bufferId;
    GLenum bufferTarget = _mufGLConvertBufferType(info->type);
    GLenum bufferAccess = _mufGLConvertAccessFlags(info->accessFlags);

    glCreateBuffers(1, &bufferId);
    glBindBuffer(bufferTarget, bufferId);
    GLbitfield storageFlags = GL_DYNAMIC_STORAGE_BIT;
    storageFlags |= info->accessFlags & MUF_ACCESS_FLAGS_READ ? GL_MAP_READ_BIT : 0;
    storageFlags |= info->accessFlags & MUF_ACCESS_FLAGS_WRITE ? GL_MAP_WRITE_BIT : 0;
    glBufferStorage(bufferTarget, (GLsizeiptr) info->size, info->data, storageFlags);
    glBindBuffer(bufferTarget, 0);

    _MufGLBuffer *buffer = mufAlloc(_MufGLBuffer, 1);
    buffer->resourceId      = bufferId;
    buffer->target          = bufferTarget;
    buffer->flags           = (GLbitfield) info->flags;
    buffer->size            = (GLsizeiptr) info->size;
    buffer->offset          = 0;
    buffer->storageFlags    = storageFlags;
    buffer->access          = bufferAccess;

    return mufMakeHandle(MufBuffer, ptr, buffer);
}

void mufGLDestroyBuffer(MufBuffer buffer) {
    _MufGLBuffer *b = mufHandleCastPtr(_MufGLBuffer, buffer);
    glDeleteBuffers(1, &b->resourceId);
    mufFree(b);
}

muf_rawptr mufGLMapBuffer(MufBuffer buffer, muf_offset offset, muf_usize size) {
    _MufGLBuffer *b = mufHandleCastPtr(_MufGLBuffer, buffer);
    MUF_ASSERT(offset + size <= b->size);

    GLboolean read = b->storageFlags & GL_MAP_READ_BIT;
    GLboolean write = b->storageFlags & GL_MAP_WRITE_BIT;
    GLenum access = read && write ? GL_READ_WRITE : (read ? GL_READ_ONLY : (write ? GL_WRITE_ONLY : GL_NONE));
    MUF_ASSERT(access != GL_NONE);

    glBindBuffer(b->target, b->resourceId);
    muf_rawptr ptr = glMapBufferRange(b->target, (GLintptr) offset, (GLsizeiptr) size, access);
    glBindBuffer(b->target, 0);
    return ptr;
}

void mufGLUnmapBuffer(MufBuffer buffer) {
    _MufGLBuffer *b = mufHandleCastPtr(_MufGLBuffer, buffer);
    glBindBuffer(b->target, b->resourceId);
    glUnmapBuffer(b->target);
}

MufSampler mufGLCreateSampler(const MufSamplerCreateInfo *info) {
    GLuint samplerId;
    glCreateSamplers(1, &samplerId);

    GLenum minFilter = _mufGLConvertTextureFilter(info->minFilter);
    GLenum magFilter = _mufGLConvertTextureFilter(info->magFilter);

    if (info->mipMapEnabled) {
        GLenum mipFilter = _mufGLConvertTextureFilter(info->minFilter);
        minFilter = _mufGLConvertMipmapTextureFilter(minFilter, mipFilter);
        magFilter = _mufGLConvertMipmapTextureFilter(magFilter, mipFilter);
    }
    glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, minFilter);
    glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, magFilter);
    
    GLenum wrapS = _mufGLConvertWrapMode(info->wrapModeU);
    GLenum wrapT = _mufGLConvertWrapMode(info->wrapModeV);
    GLenum wrapR = _mufGLConvertWrapMode(info->wrapModeW);
    glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, wrapS);
    glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, wrapT);
    glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_R, wrapR);

    glSamplerParameteri(samplerId, GL_TEXTURE_BASE_LEVEL, info->baseMipLevel);
    glSamplerParameteri(samplerId, GL_TEXTURE_MAX_LEVEL, info->maxMipLevel);

    glSamplerParameterf(samplerId, GL_TEXTURE_LOD_BIAS, info->mipLODBias);
    glSamplerParameterf(samplerId, GL_TEXTURE_MIN_LOD, info->minLOD);
    glSamplerParameterf(samplerId, GL_TEXTURE_MAX_LOD, info->maxLOD);

    glSamplerParameterfv(samplerId, GL_TEXTURE_BORDER_COLOR, &info->borderColor.r);

    GLenum compareMode = _mufGLConvertTextureCompareMode(info->compareMode);
    GLenum compareOp = _mufGLConvertCompareOperator(info->compareOp);
    glSamplerParameteri(samplerId, GL_TEXTURE_COMPARE_MODE, compareMode);
    glSamplerParameteri(samplerId, GL_TEXTURE_COMPARE_FUNC, compareOp);

    _MufGLSampler *sampler = mufAlloc(_MufGLSampler, 1);
    sampler->resourceId = samplerId;
    sampler->compareMode = compareMode;
    sampler->compareOp = compareOp;

    return mufMakeHandle(MufSampler, ptr, sampler);
}

void mufGLDestroySampler(MufSampler sampler) {
    _MufGLSampler *s = mufHandleCastPtr(_MufGLSampler, sampler);
    glDeleteSamplers(1, &s->resourceId);
    mufFree(s);
}

MufTexture mufGLCreateTexture(const MufTextureCreateInfo *info) {
    GLuint textureId;
    GLboolean isMultiSamples = info->flags & MUF_TEXTURE_FLAGS_MULTISAMPLES;
    GLboolean isCubeMap = info->flags & MUF_TEXTURE_FLAGS_CUBE_MAP;
    GLboolean isArray = info->arrayLayerCount > 1;
    GLenum textureTarget = _mufGLConvertTextureType(info->type, isMultiSamples, isArray);
    GLenum internalFormat = _mufGLConvertFormat(info->format);
    GLenum format = _mufGLConvertPixelDataFormat(info->format);
    GLenum pixelType = _mufGLConvertPixelDataType(info->format);
    GLenum access = _mufGLConvertAccessFlags(info->accessFlags);
    GLsizei sampleCount = _mufGLConvertSampleCount(info->sampleCount);

    if (isCubeMap) {
        if (info->arrayLayerCount != 0 && info->arrayLayerCount % 6 != 0) {
            mufError("The `arrayLayerCount` must be a multiple of 6 and not zero for cubemap");
        }
        if (info->type != MUF_TEXTURE_TYPE_2D) {
            mufError("The `type` must be MUF_TEXTURE_2D for cubemap");
        }
    }

    glGenTextures(1, &textureId);
    glBindTexture(textureTarget, textureId);

    switch (textureTarget) {
        case GL_TEXTURE_1D:
            glTexStorage1D(textureTarget, info->mipLevels, internalFormat, info->width);
            break;
        case GL_TEXTURE_2D:
            glTexStorage2D(textureTarget, info->mipLevels, internalFormat, info->width, info->height);
            break;
        case GL_TEXTURE_3D:
            glTexStorage3D(textureTarget, info->mipLevels, internalFormat, info->width, info->height, info->depth);
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            glTexStorage2DMultisample(textureTarget, sampleCount, internalFormat, info->width, info->height, GL_TRUE);
            break;
        case GL_TEXTURE_1D_ARRAY:
            glTexStorage2D(textureTarget, info->mipLevels, internalFormat, info->width, info->arrayLayerCount);
            break;
        case GL_TEXTURE_2D_ARRAY:
            glTexStorage3D(textureTarget, info->mipLevels, internalFormat, info->width, info->height, info->arrayLayerCount);
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            glTexStorage3DMultisample(textureTarget, sampleCount, internalFormat, info->width, info->height, info->arrayLayerCount, GL_TRUE);
            break;
    }

    _MufGLTexture *texture = mufAlloc(_MufGLTexture, 1);
    texture->resourceId         = textureId;
    texture->target             = textureTarget;
    texture->flags              = info->flags;
    texture->access             = access;
    texture->width              = info->width;
    texture->height             = info->height;
    texture->depth              = info->depth;
    texture->arrayLayerCount    = info->arrayLayerCount;
    texture->internalFormat     = internalFormat;
    texture->format             = format;
    texture->pixelType          = pixelType;
    texture->sampleCount        = sampleCount;
    texture->samplerId          = 0;

    return mufMakeHandle(MufTexture, ptr, texture);
}

void mufGLDestroyTexture(MufTexture texture) {
    _MufGLTexture *t = mufHandleCastPtr(_MufGLTexture, texture);
    glDeleteTextures(1, &t->resourceId);
    mufFree(t);
}

MufFramebuffer mufGLCreateFramebuffer(const MufFramebufferCreateInfo *info) {
    GLuint framebufferId;
    glCreateFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    GLenum colorAttachmentCounter = GL_COLOR_ATTACHMENT0;

    _MufGLFramebuffer *framebuffer = mufAlloc(_MufGLFramebuffer, 1);
    framebuffer->resourceId = framebufferId;

    for (muf_index i = 0; i < info->attachmentCount; ++i) {
        const MufAttachmentDesc *attachment = &info->attachments[i];
        MufTexture texture = attachment->texture;
        _MufGLTexture *t = mufHandleCastPtr(_MufGLTexture, texture);
        if (attachment->type == MUF_ATTACHMENT_TYPE_COLOR) {
            switch (t->target) {
                case MUF_TEXTURE_TYPE_1D:
                    glBindTexture(GL_TEXTURE_1D, t->resourceId);
                    glFramebufferTexture1D(GL_FRAMEBUFFER, colorAttachmentCounter, GL_TEXTURE_1D, t->resourceId, t->mipLevels);
                    break;
                case MUF_TEXTURE_TYPE_2D:
                    glBindTexture(GL_TEXTURE_2D, t->resourceId);
                    
                    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachmentCounter, GL_TEXTURE_2D, t->resourceId, 0);
                    break;
                case MUF_TEXTURE_TYPE_3D:
                    glBindTexture(GL_TEXTURE_3D, t->resourceId);
                    glFramebufferTexture3D(GL_FRAMEBUFFER, colorAttachmentCounter, GL_TEXTURE_3D, t->resourceId, t->mipLevels, 0);
            }
        } else if (attachment->type == MUF_ATTACHMENT_TYPE_DEPTH) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, t->resourceId, 0); 
        } else if (attachment->type == MUF_ATTACHMENT_TYPE_STENCIL) {
            glBindBuffer(GL_TEXTURE_2D, t->resourceId);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, t->resourceId, 0);
        } else if (attachment->type == MUF_ATTACHMENT_TYPE_DEPTH_STENCIL) {
            glBindBuffer(GL_TEXTURE_2D, t->resourceId);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, t->resourceId, 0);
        }
    }
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        mufError("The framebuffer is incompleted");
        return mufNullHandle(MufFramebuffer);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return mufMakeHandle(MufFramebuffer, ptr, framebuffer);
}

void mufGLDestroyFramebuffer(MufFramebuffer framebuffer) {
    _MufGLFramebuffer *f = mufHandleCastPtr(_MufGLFramebuffer, framebuffer);
    glDeleteFramebuffers(1, &f->resourceId);
    mufFree(f);
}

MufShader mufGLCreateShader(const MufShaderCreateInfo *info) {
    GLenum shaderType = _mufGLConvertShaderType(info->stageType);
    GLuint shaderId = glCreateShader(shaderType);

    _MufGLShader *shader = mufAlloc(_MufGLShader, 1);
    shader->resourceId = shaderId;
    shader->compiled = GL_TRUE;

    if (info->sourceType == MUF_SHADER_SOURCE_TYPE_TEXT) {
        const GLchar *source = (const GLchar *) info->source;
        GLint sourceSize = (GLint) info->sourceSize;
        const GLint *pSize = source == 0 ? NULL : &sourceSize;
        glShaderSource(shaderId, 1, &source, pSize);
        glCompileShader(shaderId);

        GLint status;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            shader->compiled = GL_FALSE;
            GLsizei infoLength = 0;
            glGetShaderInfoLog(shaderId, MUFGL_MAX_SHADER_INFO_LEN, &infoLength, shader->compileInfo);
            mufWarn("Failed to compile shader: %s", shader->compileInfo);
        }
    } else if (info->sourceType == MUF_SHADER_SOURCE_TYPE_BINARY) {
        glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, info->source, info->sourceSize);
    }

    return mufMakeHandle(MufShader, ptr, shader);
}

void mufGLDestroyShader(MufShader shader) {
    _MufGLShader *s = mufHandleCastPtr(_MufGLShader, shader);
    glDeleteShader(s->resourceId);
    mufFree(s);
}

MufShaderProgram mufGLCreateShaderProgram(const MufShaderProgramCreateInfo *info) {
    GLuint programId = glCreateProgram();

    _MufGLShaderProgram *program = mufAlloc(_MufGLShaderProgram, 1);
    program->resourceId = programId;

    for (muf_index i = 0; i < info->shaderCount; ++i) {
        _MufGLShader *shader = mufHandleCastPtr(_MufGLShader, info->shaders[i]);
        
        if (!shader->compiled) {
            mufWarn("The shader is not compiled in the connection stage of program, try to skip it");
            continue;
        }
        glAttachShader(programId, shader->resourceId);
    }
    glLinkProgram(programId);
    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        program->linked = GL_FALSE;
        GLsizei infoLength = 0;
        glGetProgramInfoLog(program->resourceId, MUFGL_MAX_SHADER_PROGRAM_INFO_LEN, &infoLength, program->linkInfo);
        mufError("Cannot link shader program: %s", program->linkInfo);
    }
    program->linked = GL_TRUE;
    return mufMakeHandle(MufShaderProgram, ptr, program);
}

void mufGLDestroyShaderProgram(MufShaderProgram program) {
    _MufGLShaderProgram *p = mufHandleCastPtr(_MufGLShaderProgram, program);
    glDeleteProgram(p->resourceId);
    mufFree(p);
}

MufResourceHeap mufGLCreateResourceHeap(const MufResourceHeapCreateInfo *info) {
    _MufGLResourceHeap *resourceHeap = mufAlloc(_MufGLResourceHeap, 1);
    resourceHeap->bindingCount = info->bindingCount;
    resourceHeap->bindings = mufAlloc(_MufGLResourceBindingDesc, info->bindingCount);
    for (muf_index i = 0; i < info->bindingCount; ++i) {
        MufResourceBindingDesc *desc = info->bindings + i;
        _MufGLResourceBindingDesc *glDesc = resourceHeap->bindings + i;
        glDesc->bindingIndex = desc->bindingIndex;
        glDesc->arraySize = desc->arraySize;
        glDesc->resourceType = desc->resourceType;
        if (desc->resourceType == MUF_RESOURCE_TYPE_COMBINED_TEXTURE_SAMPLER) {
            glDesc->resourcePtr0 = mufHandleCastRawptr(desc->resource.combined.texture);
            glDesc->resourcePtr1 = mufHandleCastRawptr(desc->resource.combined.sampler);
        } else if (desc->resourceType == MUF_RESOURCE_TYPE_SAMPLER) {
            glDesc->resourcePtr0 = mufHandleCastRawptr(desc->resource.sampler);
            glDesc->resourcePtr1 = NULL;
        } else if(desc->resourceType == MUF_RESOURCE_TYPE_TEXTURE || MUF_RESOURCE_TYPE_STORAGE_TEXTURE) {
            glDesc->resourcePtr0 = mufHandleCastRawptr(desc->resource.texture);
            glDesc->resourcePtr1 = NULL;
        } else {
            glDesc->resourcePtr0 = mufHandleCastRawptr(desc->resource.buffer);
            glDesc->resourcePtr1 = NULL;
        }
    }

    return mufMakeHandle(MufResourceHeap, ptr, resourceHeap);
}

void mufGLDestroyResourceHeap(MufResourceHeap resourceHeap) {
    _MufGLResourceHeap *h = mufHandleCastPtr(_MufGLResourceHeap, resourceHeap);
    mufFree(h->bindings);
    mufFree(h);
}

MufRenderPass mufGLCreateRenderPass(const MufRenderPassCreateInfo *info) {
    _MufGLRenderPass *pass = mufAlloc(_MufGLRenderPass, 1);
    GLbitfield clearBits = 0;
    
    if (info->loadOp == MUF_ATTACHMENT_LOAD_OP_CLEAR) {
        clearBits |= GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    }
    if (info->stencilLoadOp == MUF_ATTACHMENT_LOAD_OP_CLEAR) {
        clearBits |= GL_STENCIL_BUFFER_BIT;
    }

    pass->clearBits = clearBits;
    return mufMakeHandle(MufRenderPass, ptr, pass);
}

void mufGLDestroyRenderPass(MufRenderPass renderPass) {
    _MufGLRenderPass *r = (_MufGLRenderPass *) renderPass._ptr;
    mufFree(r);
}

MufPipeline mufGLCreatePipeline(const MufPipelineCreateInfo *info) {
    _MufGLPipelineState * pipeline = _mufGLPipelineStatePoolRequire(_mufGLCache->pools.pipelinePool);
    muf_u32 pipelineId = pipeline->id;

    pipeline->program = mufHandleCastPtr(_MufGLShaderProgram, info->shaderProgram)->resourceId;

    pipeline->inputAssembly.topology = _mufGLConvertPrimitiveTopology(info->primitiveType);

    if (info->inputLayout) {
        const MufVertexInputLayout *layout = info->inputLayout;
        glGenVertexArrays(1, &pipeline->vertexArray.resourceId);
        glBindVertexArray(pipeline->vertexArray.resourceId);
        for (muf_index i = 0; i < layout->attributeCount; ++i) {
            const MufVertexInputAttribute *attr = &layout->attributes[i];
            glEnableVertexAttribArray(i);
            glVertexAttribFormat(i, _mufGLGetFormatSize(attr->format), _mufGLConvertPixelDataType(attr->format),
                _mufGLIsNormalized(attr->format), attr->offset);
            glVertexAttribBinding(i, attr->bufferIndex);
        }
        glBindVertexArray(0);
        pipeline->vertexArray.baseOffset = 0;
        pipeline->vertexArray.stride = layout->stride;
    }

    if (info->viewport) {
        const MufViewport *src = info->viewport;
        _MufGLViewport *dst = &pipeline->viewport;
        dst->x = src->x;
        dst->y = src->y;
        dst->width = src->width;
        dst->height = src->height;
        dst->minDepth = src->minDepth;
        dst->maxDepth = src->maxDepth;
    }

    if (info->scissor) {
        const MufScissor *src = info->scissor;
        _MufGLScissor *dst = &pipeline->scissor;
        dst->x = src->x;
        dst->y = src->y;
        dst->width = src->width;
        dst->height = src->height;
    }

    if (info->rasterizer) {
        const MufRasterizerStateCreateInfo *src = info->rasterizer;
        _MufGLRasterizerState *dst = &pipeline->rasterizer;
        dst->cullMode = _mufGLConvertCullMode(src->cullMode);
        dst->frontFace = _mufGLConvertFrontFace(src->frontFace);
        dst->polygonMode = _mufGLConvertPolygonMode(src->polygonMode);
        dst->lineWidth = src->lineWidth;
        dst->depthClampEnabled = src->depthClampEnabled;
        dst->depthBiasEnabled = src->depthBias.enabled;
        dst->depthBiasConstantFactor = src->depthBias.constantFactor;
        dst->depthBiasClamp = src->depthBias.clamp;
        dst->depthBiasSlopFactor = src->depthBias.slopeFactor;
    }

    if (info->depth) {
        const MufDepthStateCreateInfo *src = info->depth;
        _MufGLDepthState *dst = &pipeline->depth;
        dst->testEnabled = src->testEnabled;
        dst->writeEnabled = src->writeEnabled;
        dst->compareOp = _mufGLConvertCompareOperator(src->compareOp);
    }

    if (info->stencil) {
        const MufStencilStateCreateInfo *src = info->stencil;
        _MufGLStencilState *dst = &pipeline->stencil;

        dst->testEnabled = src->testEnabled;

        dst->front.failOp = _mufGLConvertStencilOperator(src->front.failOp);
        dst->front.passOp = _mufGLConvertStencilOperator(src->front.passOp);
        dst->front.depthFailOp = _mufGLConvertStencilOperator(src->front.depthFailOp);
        dst->front.compareOp = _mufGLConvertCompareOperator(src->front.compareOp);
        dst->front.writeMask = src->front.writeMask;
        dst->front.compareMask = src->front.compareMask;
        dst->front.ref = src->front.ref;

        dst->back.failOp = _mufGLConvertStencilOperator(src->back.failOp);
        dst->back.passOp = _mufGLConvertStencilOperator(src->back.passOp);
        dst->back.depthFailOp = _mufGLConvertStencilOperator(src->back.depthFailOp);
        dst->back.compareOp = _mufGLConvertCompareOperator(src->back.compareOp);
        dst->back.writeMask = src->back.writeMask;
        dst->back.compareMask = src->back.compareMask;
        dst->back.ref = src->back.ref;
    }

    if (info->blend) {
        const MufBlendStateCreateInfo *src = info->blend;
        _MufGLBlendState *dst = &pipeline->blend;

        dst->enabled = src->enabled;

        dst->srcColorFactor = _mufGLConvertBlendFactor(src->srcColorFactor);
        dst->dstColorFactor = _mufGLConvertBlendFactor(src->dstColorFactor);
        dst->colorBlendOp = _mufGLConvertBlendOperator(src->colorBlendOp);

        dst->srcAlphaFactor = _mufGLConvertBlendFactor(src->srcAlphaFactor);
        dst->dstAlphaFactor = _mufGLConvertBlendFactor(src->dstAlphaFactor);
        dst->alphaBlendOp = _mufGLConvertBlendOperator(src->alphaBlendOp);

        dst->constant[0] = src->constant.r;
        dst->constant[1] = src->constant.g;
        dst->constant[2] = src->constant.b;
        dst->constant[3] = src->constant.a;
    }

    return mufMakeHandle(MufPipeline, u32, pipelineId);
}

void mufGLDestroyPipeline(MufPipeline pipeline) {
    _mufGLPipelineStatePoolRecycle(_mufGLCache->pools.pipelinePool, mufHandleCastU32(pipeline));
}

static MUF_INLINE void _mufGLPipelineSetCapability(GLenum capability, GLboolean enabled) {
    enabled ? glEnable(capability) : glDisable(capability);
}

static MUF_INLINE void _mufGLBindViewport(_MufGLViewport *o, const _MufGLViewport *n) {
    if (o == n) {
        return;
    }

    if (!mufMemEqual(o, n, sizeof(_MufGLScissor))) {
        glViewport(n->x, n->y, n->width, n->height);
        mufMemCopy(o, n, _MufGLScissor, 1);
    }

    if (!mufFloatEqualEpsilon(n->minDepth, o->maxDepth, MUF_EPSILON) ||
        !mufFloatEqualEpsilon(n->maxDepth, o->maxDepth, MUF_EPSILON)) {
        glDepthRange(n->minDepth, n->maxDepth);
        o->minDepth = n->minDepth;
        o->maxDepth = n->maxDepth;
    }
}

static MUF_INLINE void _mufGLBindScissor(_MufGLScissor *o, const _MufGLScissor *n) {
    if (o == n) {
        return;
    }

    if (!mufMemEqual(o, n, sizeof(_MufGLScissor))) {
        glScissor(n->x, n->y, n->width, n->height);
        mufMemCopy(o, n, _MufGLScissor, 1);
    }
}

static void _mufGLBindRasterizerState(_MufGLRasterizerState *o, const _MufGLRasterizerState *n) {
    if (o == n) {
        return;
    }

    if (n->cullMode != o->cullMode) {
        if (n->cullMode == MUF_CULL_MODE_FLAGS_NONE) {
            glDisable(GL_CULL_FACE);
        } else {
            if (o->cullMode == MUF_CULL_MODE_FLAGS_NONE) {
                glEnable(GL_CULL_FACE);
            }
        }
        glCullFace(n->cullMode);
        o->cullMode = n->cullMode;
    }

    if (n->frontFace != o->frontFace) {
        glFrontFace(n->frontFace);
        o->frontFace = n->frontFace;
    }

    if (n->polygonMode != o->polygonMode) {
        glPolygonMode(GL_FRONT_AND_BACK, n->polygonMode);
        o->polygonMode = n->polygonMode;
    }

    if (!mufFloatEqualEpsilon(n->lineWidth, o->lineWidth, MUF_EPSILON)) {
        glLineWidth(n->lineWidth);
        o->lineWidth = n->lineWidth;
    }

    if (n->depthClampEnabled != o->depthClampEnabled) {
        _mufGLPipelineSetCapability(GL_DEPTH_CLAMP, n->depthClampEnabled);
        o->depthClampEnabled = o->depthClampEnabled;
    }

    if (n->depthBiasEnabled != o->depthBiasEnabled) {
        GLenum cap[] = { GL_POLYGON_OFFSET_POINT, GL_POLYGON_OFFSET_LINE, GL_POLYGON_OFFSET_UNITS };
        _mufGLPipelineSetCapability(cap[n->polygonMode - GL_POINT], n->depthBiasEnabled);
        o->depthBiasEnabled = n->depthBiasEnabled;
    }

    if (!mufFloatEqualEpsilon(n->depthBiasConstantFactor, o->depthBiasConstantFactor, MUF_EPSILON) ||
        !mufFloatEqualEpsilon(n->depthBiasClamp, o->depthBiasClamp, MUF_EPSILON) ||
        !mufFloatEqualEpsilon(n->depthBiasSlopFactor, o->depthBiasSlopFactor, MUF_EPSILON)) {
        glPolygonOffsetClamp(n->depthBiasConstantFactor, n->depthBiasSlopFactor, n->depthBiasClamp);
        o->depthBiasConstantFactor = n->depthBiasConstantFactor;
        o->depthBiasSlopFactor = n->depthBiasSlopFactor;
        o->depthBiasClamp = n->depthBiasClamp;
    }
}

static MUF_INLINE void _mufGLBindDepthState(_MufGLDepthState *o, const _MufGLDepthState *n) {
    if (o == n) {
        return;
    }

    if (n->testEnabled != o->testEnabled) {
        _mufGLPipelineSetCapability(GL_DEPTH_TEST, n->testEnabled);
        o->testEnabled = n->testEnabled;
    }

    if (n->writeEnabled != o->writeEnabled) {
        glDepthMask(n->writeEnabled);
        o->writeEnabled = n->writeEnabled;
    }

    if (n->compareOp != o->compareOp) {
        glDepthFunc(n->compareOp);
        o->compareOp = n->compareOp;
    }
}

static MUF_INLINE void _mufGLBindStencilState(_MufGLStencilState *o, const _MufGLStencilState *n) {
    if (o == n) {
        return;
    }

    if (n->testEnabled != o->testEnabled) {
        _mufGLPipelineSetCapability(GL_STENCIL_TEST, n->testEnabled);
        o->testEnabled = n->testEnabled;
    }

    const _MufGLStencilOpState *nf = &n->front, *nb = &n->back;
    _MufGLStencilOpState *of = &o->front, *ob = &o->back;

    if (nf->writeMask != of->writeMask) {
        glStencilMaskSeparate(GL_FRONT, nf->writeMask);
        of->writeMask = nf->writeMask;
    }

    if (nb->writeMask != ob->writeMask) {
        glStencilMaskSeparate(GL_BACK, nb->writeMask);
        of->writeMask = nf->writeMask;
    }

    if (nf->compareOp != of->compareMask || nf->ref != of->ref || nf->compareMask != of->compareMask) {
        glStencilFuncSeparate(GL_FRONT, nf->compareOp, nf->ref, nf->compareMask);
        of->compareOp = nf->compareMask;
        of->ref = nf->ref;
        of->compareMask = nf->compareMask;
    }

    if (nb->compareOp != ob->compareMask || nb->ref != ob->ref || nb->compareMask != ob->compareMask) {
        glStencilFuncSeparate(GL_BACK, nb->compareOp, nb->ref, nb->compareMask);
        ob->compareOp = nb->compareMask;
        ob->ref = nb->ref;
        ob->compareMask = nb->compareMask;
    }

    if (nf->failOp != of->failOp || nf->depthFailOp != of->depthFailOp || nf->passOp != of->passOp) {
        glStencilOpSeparate(GL_FRONT, nf->failOp, nf->depthFailOp, nf->passOp);
        of->failOp = nf->failOp;
        of->depthFailOp = nf->depthFailOp;
        of->passOp = nf->passOp;
    }

    if (nb->failOp != ob->failOp || nb->depthFailOp != ob->depthFailOp || nb->passOp != ob->passOp) {
        glStencilOpSeparate(GL_BACK, nb->failOp, nb->depthFailOp, nb->passOp);
        ob->failOp = nb->failOp;
        ob->depthFailOp = nb->depthFailOp;
        ob->passOp = nb->passOp;
    }
}

static void _mufGLBindBlendState(_MufGLBlendState *o, const _MufGLBlendState *n) {
    if (o == n) {
        return;
    }

    if (n->enabled != o->enabled) {
        _mufGLPipelineSetCapability(GL_BLEND, n->enabled);
        o->enabled = n->enabled;
    }

    if (n->srcColorFactor != o->srcColorFactor ||
        n->dstColorFactor != o->dstColorFactor ||
        n->srcAlphaFactor != o->srcAlphaFactor ||
        n->dstAlphaFactor != o->dstAlphaFactor) {
        glBlendFuncSeparate(n->srcColorFactor, n->dstColorFactor,
            n->srcAlphaFactor, n->dstAlphaFactor);
        o->srcColorFactor = n->srcColorFactor;
        o->dstColorFactor = n->dstColorFactor;
        o->srcAlphaFactor = n->srcAlphaFactor;
        o->dstAlphaFactor = n->dstAlphaFactor;
    }

    if (n->colorBlendOp != o->colorBlendOp ||
        n->alphaBlendOp != o->alphaBlendOp) {
        glBlendEquationSeparate(n->colorBlendOp, n->alphaBlendOp);
        o->colorBlendOp = n->colorBlendOp;
        o->alphaBlendOp = n->alphaBlendOp;
    }

    if (!mufFloatEqualEpsilon(n->constant[0], o->constant[0], MUF_EPSILON) ||
        !mufFloatEqualEpsilon(n->constant[1], o->constant[1], MUF_EPSILON) ||
        !mufFloatEqualEpsilon(n->constant[2], o->constant[2], MUF_EPSILON) ||
        !mufFloatEqualEpsilon(n->constant[3], o->constant[3], MUF_EPSILON)) {
        glBlendColor(n->constant[0], n->constant[1], n->constant[2], n->constant[3]);
        o->constant[0] = n->constant[0];
        o->constant[1] = n->constant[1];
        o->constant[2] = n->constant[2];
        o->constant[3] = n->constant[3];
    }
}

void mufGLCmdCopyBuffer(MufBuffer dst, muf_offset dstOffset, MufBuffer src, muf_offset srcOffset, muf_usize size) {
    _MufGLBuffer *dstBuf = mufHandleCastPtr(_MufGLBuffer, dst);
    _MufGLBuffer *srcBuf = mufHandleCastPtr(_MufGLBuffer, src);
    glBindBuffer(GL_COPY_READ_BUFFER, srcBuf->resourceId);
    glBindBuffer(GL_COPY_WRITE_BUFFER, dstBuf->resourceId);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffset, dstOffset, size);
    glBindBuffer(GL_COPY_READ_BUFFER, 0);
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

void mufGLCmdUpdateBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_crawptr data) {
    _MufGLBuffer *dstBuf = mufHandleCastPtr(_MufGLBuffer, dst);
    if (!(dstBuf->storageFlags & GL_DYNAMIC_STORAGE_BIT)) {
        mufWarn("The buffer cannot be used dynamically");
        return;
    }
    glBindBuffer(dstBuf->target, dstBuf->resourceId);
    glBufferSubData(dstBuf->target, offset, size, data);
    glBindBuffer(dstBuf->target, 0);
}

void mufGLCmdFillBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_u32 data) {
    _MufGLBuffer *dstBuf = mufHandleCastPtr(_MufGLBuffer, dst);
    glBindBuffer(dstBuf->target, dstBuf->resourceId);
    glClearBufferData(dstBuf->target, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &data);
    glBindBuffer(dstBuf->target, 0);
}

void mufGLCmdCopyTexture(MufTexture dst, MufTextureCopyPos dstPos, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size) {
    _MufGLTexture *dstTex = mufHandleCastPtr(_MufGLTexture, dst);
    _MufGLTexture *srcTex = mufHandleCastPtr(_MufGLTexture, src);
    glCopyImageSubData(
        srcTex->resourceId, srcTex->target, 
        srcPos.mipLevels, srcPos.offset.x, srcPos.offset.y, srcPos.offset.z, 
        dstTex->resourceId, dstTex->target, 
        dstPos.mipLevels, dstPos.offset.x, dstPos.offset.y, dstPos.offset.z,
        size.width, size.height, size.depth
    );
#if defined(MUFGL_CORE_33)
#endif
}

void mufGLCmdCopyTextureToBuffer(MufBuffer dst, muf_usize dstOffset, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size) {
    _MufGLBuffer *dstBuf = mufHandleCastPtr(_MufGLBuffer, dst);
    _MufGLTexture *srcTex = mufHandleCastPtr(_MufGLTexture, src);
    glBindBuffer(dstBuf->target, dstBuf->resourceId);
    glBindTexture(srcTex->target, srcTex->resourceId);

}

void mufGLCmdCopyBufferToTexture(MufTexture dst, MufTextureCopyPos dstPos, MufBuffer src, muf_offset srcOffset, MufExtent3i size) {
    _MufGLTexture *dstTex = mufHandleCastPtr(_MufGLTexture, dst);
    _MufGLBuffer *srcBuf = mufHandleCastPtr(_MufGLBuffer, src);
    glBindBuffer(GL_READ_BUFFER, srcBuf->resourceId);
    glBindTexture(GL_TEXTURE_2D, dstTex->resourceId);
    glCopyTexImage2D(GL_TEXTURE_2D, dstPos.mipLevels, 0, dstPos.offset.x, dstPos.offset.y, size.width, size.height, 0);
}

void mufGLCmdGenerateMipmaps(MufTexture texture) {
    _MufGLTexture *t = mufHandleCastPtr(_MufGLTexture, texture);
    glGenerateTextureMipmap(t->resourceId);
}

void mufGLCmdUpdateTexture(MufTexture texture, MufOffset3i offset, MufExtent3i size, muf_crawptr data) {
    _MufGLTexture *t = mufHandleCastPtr(_MufGLTexture, texture);
    glBindTexture(t->target, t->resourceId);
    switch (t->target) {
        case GL_TEXTURE_1D:
            glTexSubImage1D(t->target, t->mipLevels, offset.x, size.width, t->format, t->pixelType, data);
            break;
        case GL_TEXTURE_2D:
        case GL_TEXTURE_1D_ARRAY:
            glTexSubImage2D(t->target, t->mipLevels, offset.x, offset.y, size.width, size.height, t->format, t->pixelType, data);
            break;
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
            glTexSubImage3D(t->target, t->mipLevels, offset.x, offset.y, offset.z, size.width, size.height, size.depth, t->format, t->pixelType, data);
            break;
    }
}

void mufGLCmdSetPrimitiveTopology(MufPrimitiveTopology topology) {
    _mufGLCache->pipeline->inputAssembly.topology = topology;
}

void mufGLCmdSetPrimitiveRestartEnabled(muf_bool enabled) {
    _mufGLCache->pipeline->inputAssembly.primitiveRestartEnabled = enabled;
}

void mufGLCmdSetViewport(const MufViewport *viewport) {
    _MufGLViewport tmp = {
        .x = viewport->x,
        .y = viewport->y,
        .width = viewport->width,
        .height = viewport->height,
        .minDepth = viewport->minDepth,
        .maxDepth = viewport->maxDepth
    };
    _mufGLBindViewport(&_mufGLCache->pipeline->viewport, &tmp);
}

void mufGLCmdSetScissor(const MufScissor *scissor) {
    _MufGLScissor tmp = {
        .x = scissor->x,
        .y = scissor->y,
        .width = scissor->width,
        .height = scissor->height,
    };
    _mufGLBindScissor(&_mufGLCache->pipeline->scissor, &tmp);
}

void mufGLCmdSetLineWidth(muf_f32 width) {
    glLineWidth(width);
    _mufGLCache->pipeline->rasterizer.lineWidth = width;
}

void mufGLCmdSetStencilWriteMask(MufStencilFaceFlags face, muf_u32 mask) {
    if (face & MUF_STENCIL_FACE_FLAGS_FRONT) {
        glStencilMaskSeparate(GL_FRONT, mask);
        _mufGLCache->pipeline->stencil.front.writeMask = mask;
    }
    if (face & MUF_STENCIL_FACE_FLAGS_FRONT) {
        glStencilMaskSeparate(GL_BACK, mask);
        _mufGLCache->pipeline->stencil.back.writeMask = mask;
    }
}

void mufGLCmdSetStencilCompareMask(MufStencilFaceFlags face, muf_u32 mask) {
    if (face & MUF_STENCIL_FACE_FLAGS_FRONT) {
        _MufGLStencilOpState *s = &_mufGLCache->pipeline->stencil.front;
        glStencilFuncSeparate(GL_FRONT, s->compareOp, s->ref, mask);
        s->compareMask = mask;
    }
    if (face & MUF_STENCIL_FACE_FLAGS_FRONT) {
        _MufGLStencilOpState *s = &_mufGLCache->pipeline->stencil.back;
        glStencilFuncSeparate(GL_BACK, s->compareOp, s->ref, mask);
        s->compareMask = mask;
    }
}

void mufGLCmdSetStencilRef(MufStencilFaceFlags face, muf_u32 ref) {
    if (face & MUF_STENCIL_FACE_FLAGS_FRONT) {
        _MufGLStencilOpState *s = &_mufGLCache->pipeline->stencil.front;
        glStencilFuncSeparate(GL_FRONT, s->compareOp, ref, s->compareMask);
        s->ref= ref;
    }
    if (face & MUF_STENCIL_FACE_FLAGS_FRONT) {
        _MufGLStencilOpState *s = &_mufGLCache->pipeline->stencil.back;
        glStencilFuncSeparate(GL_BACK, s->compareOp, ref, s->compareMask);
        s->ref = ref;
    }
}

void mufGLCmdSetBlendConstant(MufRGBA rgba) {
    glBlendColor(rgba.r, rgba.g, rgba.b, rgba.a);
    _MufGLBlendState *s = &_mufGLCache->pipeline->blend;
    s->constant[0] = rgba.r;
    s->constant[1] = rgba.g;
    s->constant[2] = rgba.b;
    s->constant[3] = rgba.a;
}

void mufGLCmdSetClearColor(MufRGBA rgba) {
    glClearColor(rgba.r, rgba.g, rgba.b, rgba.a);
}

void mufGLCmdSetClearDepth(muf_f32 depth) {
    glClearDepth(depth);
}

void mufGLCmdSetClearStencil(muf_u32 stencil) {
    glClearStencil(stencil);
}

void mufGLCmdBindPipeline(MufPipeline pipeline) {
    muf_u32 id = mufHandleCastU32(pipeline);
    _MufGLPipelineState *newPipeline = _mufGLPipelineStatePoolGet(_mufGLCache->pools.pipelinePool, id);
    _MufGLPipelineState *cachePipeline = _mufGLCache->pipeline;

    if (newPipeline == cachePipeline) {
        return;
    }

    if (newPipeline->program != cachePipeline->program) {
        cachePipeline->program = newPipeline->program;
        glUseProgram(cachePipeline->program);
    }

    _mufGLBindViewport(&cachePipeline->viewport, &newPipeline->viewport);
    _mufGLBindScissor(&cachePipeline->scissor, &newPipeline->scissor);
    _mufGLBindRasterizerState(&cachePipeline->rasterizer, &newPipeline->rasterizer);
    _mufGLBindDepthState(&cachePipeline->depth, &newPipeline->depth);
    _mufGLBindStencilState(&cachePipeline->stencil, &newPipeline->stencil);
    _mufGLBindBlendState(&cachePipeline->blend, &newPipeline->blend);
    _mufGLCache->pipeline = newPipeline;
}

void mufGLCmdBindVertexBuffers(const MufBuffer *buffers, const muf_offset *offsets, 
    muf_index firstBindingIndex, muf_usize bindingCount) {
    glBindVertexArray(_mufGLCache->pipeline->vertexArray.resourceId);
    GLuint *bufferIds = mufAlloc(GLuint, bindingCount);
    GLsizei *strides = mufAlloc(GLsizei, bindingCount);
    for (muf_index i = 0; i < bindingCount; ++i) {
        _MufGLBuffer *b = mufHandleCastPtr(_MufGLBuffer, buffers[i]);
        bufferIds[i] = b->resourceId;
        strides[i] = _mufGLCache->pipeline->vertexArray.stride;
    }
    glBindVertexBuffers(firstBindingIndex, bindingCount, bufferIds, (GLintptr *) offsets, strides);
    mufFree(bufferIds);
    mufFree(strides);
}

void mufGLCmdBindVertexBuffer(MufBuffer buffer, muf_offset offset, muf_index bindingIndex) {
    _MufGLBuffer *b = mufHandleCastPtr(_MufGLBuffer, buffer);
    glBindVertexArray(_mufGLCache->pipeline->vertexArray.resourceId);
    glBindVertexBuffer(bindingIndex, b->resourceId, offset, _mufGLCache->pipeline->vertexArray.stride);
    glBindVertexArray(0);
}

void mufGLCmdBindIndexBuffer(MufBuffer buffer, muf_offset offset) {
    _MufGLBuffer *b = mufHandleCastPtr(_MufGLBuffer, buffer);
    glVertexArrayElementBuffer(_mufGLCache->pipeline->vertexArray.resourceId, b->resourceId);
    _mufGLCache->pipeline->vertexArray.indexBuffer = b;
}

void mufGLCmdBindResourceHeap(MufResourceHeap heap) {
    _MufGLResourceHeap *h = mufHandleCastPtr(_MufGLResourceHeap, heap);

    for (muf_index i = 0; i < h->bindingCount; ++i) {
        _MufGLResourceBindingDesc *res = h->bindings + i;
        switch (res->resourceType) {
            case MUF_RESOURCE_TYPE_SAMPLER: {

            } break;

            case MUF_RESOURCE_TYPE_TEXTURE: {
                _MufGLTexture *texture = (_MufGLTexture *) res->resourcePtr0;
                glActiveTexture(GL_TEXTURE0 + res->bindingIndex);
                glBindTexture(texture->target, texture->resourceId);
            } break;

            case MUF_RESOURCE_TYPE_COMBINED_TEXTURE_SAMPLER: {
                _MufGLTexture *texture = (_MufGLTexture *) res->resourcePtr0;
                _MufGLSampler *sampler = (_MufGLSampler *) res->resourcePtr1;
                glActiveTexture(GL_TEXTURE0 + res->bindingIndex);
                glBindTexture(texture->target, texture->resourceId);
                glBindSampler(res->bindingIndex, sampler->resourceId);
            } break;

            case MUF_RESOURCE_TYPE_STORAGE_TEXTURE: {
                _MufGLTexture *texture = (_MufGLTexture *) res->resourcePtr0;
                MUF_ASSERT(texture->flags & MUF_TEXTURE_FLAGS_STORAGE);
                glBindImageTexture(res->bindingIndex, texture->resourceId, texture->mipLevels,
                    texture->arrayLayerCount > 1, 1, texture->access, texture->format);
            } break;

            case MUF_RESOURCE_TYPE_STORAGE_BUFFER: {
                _MufGLBuffer *buffer = (_MufGLBuffer *) res->resourcePtr0;
                MUF_ASSERT(buffer->target == GL_SHADER_STORAGE_BUFFER);
                glBindBuffer(buffer->target, buffer->resourceId);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, res->bindingIndex, buffer->resourceId);
                glBindBuffer(buffer->target, 0);

            } break;
 
            case MUF_RESOURCE_TYPE_UNIFOM_BUFFER: {
                _MufGLBuffer *buffer = (_MufGLBuffer *) res->resourcePtr0;
                MUF_ASSERT(buffer->target == GL_UNIFORM_BUFFER);
                glBindBuffer(buffer->target, buffer->resourceId);
                glBindBufferBase(GL_UNIFORM_BUFFER, res->bindingIndex, buffer->resourceId);
                glBindBuffer(buffer->target, 0);
            } break;
            default: MUF_UNREACHABLE();
        }    
    }
}

void mufGLCmdBeginRenderPass(MufRenderPass renderPass) {
    _MufGLRenderPass *r = mufHandleCastPtr(_MufGLRenderPass, renderPass);
    glClear(r->clearBits);
    _mufGLCache->renderPass = r;
}

void mufGLCmdEndRenderPass() {
    _mufGLCache->renderPass = NULL;
}

void mufGLCmdDraw(muf_index firstIndex, muf_index count) {
    _MufGLPipelineState *p = _mufGLCache->pipeline;
    glUseProgram(p->program);
    glBindVertexArray(p->vertexArray.resourceId);
    if (p->inputAssembly.primitiveRestartEnabled) {
        glPrimitiveRestartIndex(0xFFFFFFFF);
    }
    glDrawArrays(p->inputAssembly.topology, firstIndex, count);
}

void mufGLCmdDrawIndexed(muf_index firstIndex, muf_usize count) {
    _MufGLPipelineState *p = _mufGLCache->pipeline;
    glUseProgram(p->program);
    
    const _MufGLVertexArray *vao = &p->vertexArray;
    GLenum indexType = vao->indexBuffer->flags & MUF_BUFFER_FLAGS_INDEX16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

    glBindVertexArray(p->vertexArray.resourceId);
    if (p->inputAssembly.primitiveRestartEnabled) {
        if (indexType == GL_UNSIGNED_SHORT) {
            glPrimitiveRestartIndex(0xFFFF);
        } else {
            glPrimitiveRestartIndex(0xFFFFFFFF);
        }
    }
    glDrawRangeElements(p->inputAssembly.topology, firstIndex, firstIndex + count, count, indexType, NULL);
}

static void mufGLInit() {
    //_mufGLinitConfig();
    _mufGLInitCache();
}

static void mufGLFinish() {
    _mufGLFinishCache();
}

MufRenderBackendRegistry _glBackendRegistry[1] = {{
    .name = "opengl-core-4.5",
    .init = mufGLInit,
    .finish = mufGLFinish,
    .api = {
        .createBuffer           = mufGLCreateBuffer,
        .destroyBuffer          = mufGLDestroyBuffer,
        .createSampler          = mufGLCreateSampler,
        .destroySampler         = mufGLDestroySampler,
        .createTexture          = mufGLCreateTexture,
        .destroyTexture         = mufGLDestroyTexture,
        .createFramebuffer      = mufGLCreateFramebuffer,
        .destroyFramebuffer     = mufGLDestroyFramebuffer,
        .createShader           = mufGLCreateShader,
        .destroyShader          = mufGLDestroyShader,
        .createShaderProgram    = mufGLCreateShaderProgram,
        .destroyShaderProgram   = mufGLDestroyShaderProgram,
        .createPipeline         = mufGLCreatePipeline,
        .destroyPipeline        = mufGLDestroyPipeline,
        .createRenderPass       = mufGLCreateRenderPass,
        .destroyRenderPass      = mufGLDestroyRenderPass,
        .createResourceHeap     = mufGLCreateResourceHeap,
        .destroyResourceHeap    = mufGLDestroyResourceHeap,

        .cmd = {
            .copyBuffer                 = mufGLCmdCopyBuffer,
            .updateBuffer               = mufGLCmdUpdateBuffer,
            .fillBuffer                 = mufGLCmdFillBuffer,
            .copyTexture                = mufGLCmdCopyTexture,
            .copyTextureToBuffer        = mufGLCmdCopyTextureToBuffer,
            .copyBufferToTexture        = mufGLCmdCopyBufferToTexture,
            .generateMipmaps            = mufGLCmdGenerateMipmaps,
            .updateTexture              = mufGLCmdUpdateTexture,
            .setPrimitiveTopology       = mufGLCmdSetPrimitiveTopology,
            .setPrimitiveRestartEnabled = mufGLCmdSetPrimitiveRestartEnabled,
            .setViewport                = mufGLCmdSetViewport,
            .setScissor                 = mufGLCmdSetScissor,
            .setLineWidth               = mufGLCmdSetLineWidth,
            .setStencilWriteMask        = mufGLCmdSetStencilWriteMask,
            .setStencilCompareMask      = mufGLCmdSetStencilCompareMask,
            .setStencilRef              = mufGLCmdSetStencilRef,
            .setBlendConstant           = mufGLCmdSetBlendConstant,
            .setClearColor              = mufGLCmdSetClearColor,
            .setClearDepth              = mufGLCmdSetClearDepth,
            .setClearStencil            = mufGLCmdSetClearStencil,
            .bindPipeline               = mufGLCmdBindPipeline,
            .bindVertexBuffers          = mufGLCmdBindVertexBuffers,
            .bindVertexBuffer           = mufGLCmdBindVertexBuffer,
            .bindIndexBuffer            = mufGLCmdBindIndexBuffer,
            .bindResourceHeap           = mufGLCmdBindResourceHeap,
            .beginRenderPass            = mufGLCmdBeginRenderPass,
            .endRenderPass              = mufGLCmdEndRenderPass,
            .draw                       = mufGLCmdDraw,
            .drawIndexed                = mufGLCmdDrawIndexed
        }
    }
}};

static void _load(void) {
    mufRegisterRenderBackend(_glBackendRegistry);
}

static void _unload(void) {

}

MUF_MODULE_REGISTRY_DEF(Render_GL,
    "MF_RenderBackend_gl_core45",
    "",
    mufCreateVersion(1, 0, 0),
    0,
    _load,
    _unload
);