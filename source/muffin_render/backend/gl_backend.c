#include "muffin_render/backend/gl_backend.h"

typedef struct _MufGLViewport_s {
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
    GLfloat minDepth;
    GLfloat maxDepth;
} _MufGLViewport;

typedef struct _MufGLScissor_s {
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
} _MufGLScissor;

typedef struct _MufGLDepthState_s {
    GLboolean   testEnabled;
    GLboolean   writeEnabled;
    GLenum      compareOp;
} _MufGLDepthState;

typedef struct _MufGLStencilState_s {
    GLboolean   testEnabled;
    GLenum      failOp;
    GLenum      passOp;
    GLenum      depthFailOp;
    GLenum      compareOp;
    GLuint      compareMask;
    GLuint      writeMask;
    GLuint      ref;
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

typedef struct _MufGLPipelineState_s {
    _MufGLCapabilityFlags   capabilities;

    GLuint                  program;
    MufVertexInputLayout    inputLayout;
    _MufGLViewport          viewport;
    _MufGLScissor           scissor;
    _MufGLDepthState        depth;
    _MufGLStencilState      stencil;
    _MufGLBlendState        blend;
    _MufGLRasterizerState   rasterizer;
} _MufGLPipelineState;

#define MUF_GL_PIPELINE_POOL_MAX_SIZE 8

typedef struct MufGLPipelineStatePool_s {
    _MufGLPipelineState states[MUF_GL_PIPELINE_POOL_MAX_SIZE];
    muf_u32 bitMap;
} MufGLPipelinePool;

MufGLPipelinePool _mufGLPipelines;

typedef struct _MufGLStateCache_s {
    _MufGLPipelineState pipeline;
} _MufGLStateCache;

_MufGLStateCache _glCache;

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetBufferType(MufBufferType type) {
    switch (type) {
        case MUF_BUFFER_TYPE_VERTEX:    return GL_ARRAY_BUFFER;
        case MUF_BUFFER_TYPE_INDEX:     return GL_ELEMENT_ARRAY_BUFFER;
        case MUF_BUFFER_TYPE_UNIFORM:   return GL_UNIFORM_BUFFER;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetBufferUsage(MufBufferUsage usage) {
    switch (usage) {
        case MUF_BUFFER_USAGE_STATIC:   return GL_STATIC_DRAW;
        case MUF_BUFFER_USAGE_DYNAMIC:  return GL_DYNAMIC_DRAW;
        case MUF_BUFFER_USAGE_STREAM:   return GL_STREAM_DRAW;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetShaderType(MufShaderType type) {
    switch (type) {
        case MUF_SHADER_TYPE_VERTEX:    return GL_VERTEX_SHADER;
        case MUF_SHADER_TYPE_FRAGMENT:  return GL_FRAGMENT_SHADER;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetFormat(MufFormat format) {
    switch (format) {
        case MUF_FORMAT_F32: return GL_FLOAT;
    }
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetCullMode(MufCullModeFlags mode) {
    muf_bool back = mode & MUF_CULL_MODE_BACK;
    muf_bool front = mode & MUF_CULL_MODE_FRONT;

    if (back && front)
        return GL_FRONT_AND_BACK;
    if (back)
        return GL_BACK;
    if (front)
        return GL_BACK;
    MUF_UNREACHABLE();
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetFrontFace(MufFrontFace mode) {
    switch (mode) {
        case MUF_FRONT_FACE_CW:     return GL_CW;
        case MUF_FRONT_FACE_CCW:    return GL_CCW;
        default:                    MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetPolygonMode(MufPolygonMode mode) {
    switch (mode) {
        case MUF_POLYGON_MODE_POINT:    return GL_POINT;
        case MUF_POLYGON_MODE_LINE:     return GL_LINE;
        case MUF_POLYGON_MODE_FILL:     return GL_FILL;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetPrimitiveType(MufPrimitiveType primitive) {
    switch (primitive) {
        case MUF_PRIMITIVE_TYPE_POINTS:         return GL_POINTS;
        case MUF_PRIMITIVE_TYPE_LINES:          return GL_LINES;
        case MUF_PRIMITIVE_TYPE_LINE_STRIP:     return GL_LINE_STRIP;
        case MUF_PRIMITIVE_TYPE_TRIANGLES:      return GL_TRIANGLES;
        case MUF_PRIMITIVE_TYPE_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        default:                                MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetCompareOperator(MufCompareOperator op) {
    switch (op) {
        case MUF_COMPARE_OP_ALWAYS:     return GL_ALWAYS;
        case MUF_COMPARE_OP_NEVER:      return GL_NEVER;
        case MUF_COMPARE_OP_LESS:       return GL_LESS;
        case MUF_COMPARE_OP_GREATER:    return GL_GREATER;
        case MUF_COMPARE_OP_EQUAL:      return GL_EQUAL;
        case MUF_COMPARE_OP_NEQUAL:     return GL_NOTEQUAL;
        case MUF_COMPARE_OP_LEQAUL:     return GL_LEQUAL;
        case MUF_COMPARE_OP_GEQUAL:     return GL_GEQUAL;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetStencilOperator(MufStencilOperator op) {
    switch (op) {
        case MUF_STENCIL_OP_KEEP:       return GL_KEEP;
        case MUF_STENCIL_OP_ZERO:       return GL_ZERO;
        case MUF_STENCIL_OP_REPLACE:    return GL_REPLACE;
        case MUF_STENCIL_OP_INC:        return GL_INCR;
        case MUF_STENCIL_OP_WARP_INC:   return GL_INCR_WRAP;
        case MUF_STENCIL_OP_DEC:        return GL_DECR;
        case MUF_STENCIL_OP_WRAP_DEC:   return GL_DECR_WRAP;
        case MUF_STENCIL_OP_INVERT:     return GL_INVERT;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLuint _mufGLGetColorMask(MufColorMask mask) {

}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetBlendOperator(MufBlendOperator op) {
    switch (op) {
        case MUF_BLEND_OP_ADD:          return GL_FUNC_ADD;
        case MUF_BLEND_OP_SUB:          return GL_FUNC_SUBTRACT;
        case MUF_BLEND_OP_REVERSE_SUB:  return GL_FUNC_REVERSE_SUBTRACT;
        default:                        MUF_UNREACHABLE();
    }
    return -1;
}

MUF_INTERNAL MUF_INLINE GLenum _mufGLGetBlendFactor(MufBlendFactor factor) {
    switch (factor) {
        case MUF_BLEND_FACTOR_ZERO:                     return GL_ZERO;
        case MUF_BLEND_FACTOR_ONE:                      return GL_ONE;
        case MUF_BLEND_FACTOR_SRC_COLOR:                return GL_SRC_COLOR;
        case MUF_BLEND_FACTOR_DST_COLOR:                return GL_DST_COLOR;
        case MUF_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:      return GL_ONE_MINUS_SRC_COLOR;
        case MUF_BLEND_FACTOR_ONE_MINUS_DST_COLOR:      return GL_ONE_MINUS_DST_COLOR;
        case MUF_BLEND_FACTOR_SRC_ALPHA:                return GL_SRC_ALPHA;
        case MUF_BLEND_FACTOR_DST_ALPHA:                return GL_DST_ALPHA;
        case MUF_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:      return GL_ONE_MINUS_SRC_ALPHA;
        case MUF_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:      return GL_ONE_MINUS_DST_ALPHA;
        case MUF_BLEND_FACTOR_CONSTANT:                 return GL_CONSTANT_COLOR;
        case MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT:       return GL_ONE_MINUS_CONSTANT_COLOR;
        case MUF_BLEND_FACTOR_CONSTANT_ALPHA:           return GL_CONSTANT_ALPHA;
        case MUF_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
        default:                                        MUF_UNREACHABLE();
    }
    return -1;
}

MufBuffer mufGLCreateBuffer(const MufBufferCreateInfo *info) {
    GLuint bufferId;
    GLenum bufferType = _mufGLGetBufferType(info->type);
    GLenum bufferUsage = _mufGLGetBufferUsage(info->usage);

    glCreateBuffers(1, &bufferId);
    glBindBuffer(bufferType, bufferId);
    glBufferData(bufferType, info->size, info->data, bufferUsage);
    glBindBuffer(bufferType, 0);

    return mufCreateHandle(u32, bufferId);
}

void mufGLDestroyBuffer(MufBuffer buffer) {
    glDeleteBuffers(1, &buffer._u32);
}

void mufGLBufferUpdate(MufBuffer buffer, muf_usize offset, const muf_rawptr data, muf_usize size) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer._u32);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void mufGLBufferClear(MufBuffer buffer, MufFormat internalForamt, MufFormat format, const muf_rawptr data) {
}

MufShader mufGLCreateShader(const MufShaderCreateInfo *info) {
    GLenum shaderType = _mufGLGetShaderType(info->stageType);
    GLuint shaderId = glCreateShader(shaderType);

    if (info->sourceType == MUF_SHADER_SOURCE_TYPE_TEXT) {
        glShaderSource(shaderId, 1, (const GLchar *) info->source, info->sourceSize);
        glCompileShader(shaderId);
    } else if (info->sourceType == MUF_SHADER_SOURCE_TYPE_BINARY) {
        glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, info->source, info->sourceSize);
    } else {
        return MUF_NULL_HANDLE;
    }
    return mufCreateHandle(u32, shaderId);
}

void mufGLDestroyShader(MufShader shader) {
    glDeleteShader(shader._u32);
}

MufShaderProgram mufGLCreateShaderProgram(const MufShaderProgramCreateInfo *info) {
    GLuint programId = glCreateProgram();

    for (muf_index i = 0; i < info->shaderCount; ++i) {
        glAttachShader(programId, info->shaders[i]._u32);    
    }
    glLinkProgram(programId);

    if (info->autoDestroy) {
        for (muf_index i = 0; i < info->shaderCount; ++i) {
            mufGLDestroyShader(info->shaders[i]);
        }
    }

    return mufCreateHandle(u32, programId);
}

void mufGLDestroyShaderProgram(MufShaderProgram program) {
    glDeleteProgram(program._u32);
}

MUF_INTERNAL void _mufGLBindViewportState() {

}

MUF_INTERNAL void _mufGLBindBlendState() {

    
}

muf_u32 _mufGLPipelineStatePoolSpawn(MufGLPipelinePool *pool, const MufPipelineCreateInfo *info) {
    muf_u32 id = 0;

    _MufGLPipelineState *state = &pool->states[id];
    state->program = info->shaderProgram->_u32;

    const MufViewport *viewport = info->viewport;
    state->viewport.x           = info->viewport->x;
    state->viewport.y           = info->viewport->y;
    state->viewport.width       = info->viewport->width;
    state->viewport.height      = info->viewport->height;
    state->viewport.minDepth    = info->viewport->minDepth;
    state->viewport.maxDepth    = info->viewport->maxDepth;

    const MufScissor *scissor = info->scissor;
    state->scissor.x            = scissor->x;
    state->scissor.y            = scissor->y;
    state->scissor.width        = scissor->width;
    state->scissor.height       = scissor->height;

    const MufRasterizerStateCreateInfo *rasterizer = info->rasterizer;
    state->rasterizer.cullMode                  = _mufGLGetCullMode(rasterizer->cullMode);
    state->rasterizer.frontFace                 = _mufGLGetFrontFace(rasterizer->frontFace);
    state->rasterizer.polygonMode               = _mufGLGetPolygonMode(rasterizer->polygonMode);
    state->rasterizer.lineWidth                 = rasterizer->lineWidth;
    state->rasterizer.depthClampEnabled         = rasterizer->depthClampEnabled;
    state->rasterizer.depthBiasConstantFactor   = rasterizer->depthBias->constantFactor;
    state->rasterizer.depthBiasClamp            = rasterizer->depthBias->clamp;
    state->rasterizer.depthBiasSlopFactor       = rasterizer->depthBias->slopeFactor;
    state->rasterizer.multiSampleEnabled        = rasterizer->multiSamplingEnabled;
    state->rasterizer.multiSampleCount          = rasterizer->multiSampleCount;

    const MufDepthStateCreateInfo *depth = info->depth;
    state->depth.testEnabled    = depth->testEnabled;
    state->depth.writeEnabled   = depth->writeEnabled;
    state->depth.compareOp      = _mufGLGetCompareOperator(depth->compareOp);

    const MufStencilStateCreateInfo *stencil = info->stencil;
    state->stencil.testEnabled  = stencil->testEnabled;
    state->stencil.failOp       = _mufGLGetStencilOperator(stencil->failOp);
    state->stencil.passOp       = _mufGLGetStencilOperator(stencil->passOp);
    state->stencil.depthFailOp  = _mufGLGetStencilOperator(stencil->depthFailOp);
    state->stencil.compareOp    = _mufGLGetCompareOperator(stencil->compareOp);
    state->stencil.compareMask  = stencil->compareMask;
    state->stencil.writeMask    = stencil->writeMask;
    state->stencil.ref          = stencil->ref;

    const MufBlendStateCreateInfo *blend = info->blend;
    state->blend.srcColorFactor = _mufGLGetBlendFactor(blend->srcColorFactor);
    state->blend.dstColorFactor = _mufGLGetBlendFactor(blend->dstColorFactor);
    state->blend.colorBlendOp   = _mufGLGetBlendOperator(blend->colorBlendOp);
    state->blend.srcAlphaFactor = _mufGLGetBlendFactor(blend->srcAlphaFactor);
    state->blend.dstAlphaFactor = _mufGLGetBlendFactor(blend->dstAlphaFactor);
    state->blend.alphaBlendOp   = _mufGLGetBlendOperator(blend->alphaBlendOp);
    state->blend.constant[0]    = blend->blendConstant.r;
    state->blend.constant[1]    = blend->blendConstant.g;
    state->blend.constant[2]    = blend->blendConstant.b;
    state->blend.constant[3]    = blend->blendConstant.a;

    return id;
}

MufPipeline mufGLCreatePipeline(const MufPipelineCreateInfo *info) {
    const MufBlendStateCreateInfo *blend = info->blend;

    muf_u32 pipelineId = _mufGLPipelineStatePoolSpawn(&_mufGLPipelines, info);

    return mufCreateHandle(u32, pipelineId);
}

void mufGLDestroyPipeline(MufPipeline pipeline) {

}

void mufGLBindPipeline(MufPipeline pipeline) {
    muf_index index = pipeline._u32;

    _MufGLPipelineState *state = &_mufGLPipelines.states[index];
    _MufGLPipelineState *cache = &_glCache.pipeline;

    _MufGLDepthState *depth = &state->depth;
    if (depth->testEnabled != cache->depth.testEnabled) {

    }

    if (depth->writeEnabled != cache->depth.writeEnabled) {
        glDepthMask(depth->writeEnabled);        
    }

    if (depth->compareOp != cache->depth.compareOp) {
        glDepthFunc(depth->compareOp);
    }

    _MufGLStencilState *stencil = &state->stencil;
    if (stencil->failOp != cache->stencil.failOp || 
        stencil->passOp != cache->stencil.passOp ||
        stencil->depthFailOp != cache->stencil.depthFailOp) {
        glStencilOp(stencil->failOp, stencil->depthFailOp, stencil->passOp);
    }

    if (stencil->compareOp != cache->stencil.compareOp ||
        stencil->compareMask != cache->stencil.compareMask ||
        stencil->ref != cache->stencil.ref) {
        glStencilFunc(stencil->compareOp, stencil->ref, stencil->compareMask);
    }

    if (stencil->writeMask != cache->stencil.writeMask) {
        glStencilMask(stencil->writeMask);
    }

    _MufGLBlendState *blend = &state->blend;
    if (blend->srcColorFactor != cache->blend.srcColorFactor ||
        blend->dstColorFactor != cache->blend.dstColorFactor ||
        blend->srcAlphaFactor != cache->blend.srcAlphaFactor ||
        blend->dstAlphaFactor != cache->blend.dstAlphaFactor) {
        glBlendFuncSeparate(blend->srcColorFactor,blend->dstColorFactor,
            blend->srcAlphaFactor, blend->dstAlphaFactor);
        cache->blend.srcColorFactor = blend->srcColorFactor;
        cache->blend.dstColorFactor = blend->dstColorFactor;
        cache->blend.srcAlphaFactor = blend->srcAlphaFactor;
        cache->blend.dstAlphaFactor = blend->dstAlphaFactor;
    }

    if (blend->colorBlendOp != cache->blend.colorBlendOp ||
        blend->alphaBlendOp != cache->blend.alphaBlendOp) {
        glBlendEquationSeparate(blend->colorBlendOp, blend->alphaBlendOp);
        cache->blend.colorBlendOp = blend->colorBlendOp;
        cache->blend.alphaBlendOp = blend->alphaBlendOp;
    }

    if (blend->constant[0] != cache->blend.constant[0] ||
        blend->constant[1] != cache->blend.constant[1] ||
        blend->constant[2] != cache->blend.constant[2] ||
        blend->constant[3] != cache->blend.constant[3]) {
        glBlendColor(blend->constant[0], blend->constant[1],
            blend->constant[2], blend->constant[1]);
        cache->blend.constant[0] = blend->constant[0];
        cache->blend.constant[1] = blend->constant[1];
        cache->blend.constant[2] = blend->constant[2];
        cache->blend.constant[3] = blend->constant[3];
    }


}