#ifndef _MUFFIN_RENDER_BACKEND_H_
#define _MUFFIN_RENDER_BACKEND_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"

MUF_STRUCT_TYPEDEF(MufBufferCreateInfo);
MUF_STRUCT_TYPEDEF(MufSamplerCreateInfo);
MUF_STRUCT_TYPEDEF(MufTextureCreateInfo);
MUF_STRUCT_TYPEDEF(MufFramebufferCreateInfo);
MUF_STRUCT_TYPEDEF(MufShaderCreateInfo);
MUF_STRUCT_TYPEDEF(MufShaderProgramCreateInfo);
MUF_STRUCT_TYPEDEF(MufPipelineCreateInfo);
MUF_STRUCT_TYPEDEF(MufRenderPassCreateInfo);
MUF_STRUCT_TYPEDEF(MufResourceHeapCreateInfo);

MUF_STRUCT_TYPEDEF(MufViewport);
typedef MufRect2i MufScissor;
MUF_STRUCT_TYPEDEF(MufTextureCopyPos);

MUF_ENUM_TYPEDEF(MufStencilFaceFlags);
MUF_ENUM_TYPEDEF(MufPrimitiveTopology);

MUF_UNION_TYPEDEF(MufBuffer);
MUF_UNION_TYPEDEF(MufSampler);
MUF_UNION_TYPEDEF(MufTexture);
MUF_UNION_TYPEDEF(MufFramebuffer);
MUF_UNION_TYPEDEF(MufShader);
MUF_UNION_TYPEDEF(MufShaderProgram);
MUF_UNION_TYPEDEF(MufPipeline);
MUF_UNION_TYPEDEF(MufRenderPass);
MUF_UNION_TYPEDEF(MufResourceHeap);

MUF_HANDLE_DEF(MufRenderBackend);

enum {
    MUF_RENDER_BACKEND_MAX_NAME_LEN = 32
};

typedef struct MufRenderBackendApi_s {
    MufBuffer (* createBuffer)(const MufBufferCreateInfo *info);
    void (* destroyBuffer)(MufBuffer buffer);
    muf_rawptr (* mapBuffer)(MufBuffer buffer, muf_offset offset, muf_usize size);
    void (* unmapBuffer)(MufBuffer buffer);

    MufSampler (* createSampler)(const MufSamplerCreateInfo *info);
    void (* destroySampler)(MufSampler sampler);

    MufTexture (* createTexture)(const MufTextureCreateInfo *info);
    void (* destroyTexture)(MufTexture texture);

    MufFramebuffer (* createFramebuffer)(const MufFramebufferCreateInfo *info);
    void (* destroyFramebuffer)(MufFramebuffer frameBuffer);

    MufShader (* createShader)(const MufShaderCreateInfo *info);
    void (* destroyShader)(MufShader shader);

    MufShaderProgram (* createShaderProgram)(const MufShaderProgramCreateInfo *info);
    void (* destroyShaderProgram)(MufShaderProgram program);

    MufPipeline (* createPipeline)(const MufPipelineCreateInfo *info);
    void (* destroyPipeline)(MufPipeline pipeline);

    MufRenderPass (* createRenderPass)(const MufRenderPassCreateInfo *info);
    void (* destroyRenderPass)(MufRenderPass renderPass);

    MufResourceHeap (* createResourceHeap)(const MufResourceHeapCreateInfo *info);
    void (* destroyResourceHeap)(MufResourceHeap resourceHeap);

    struct _Commands {
        void (* copyBuffer)(MufBuffer dst, muf_offset dstOffset, MufBuffer src, muf_offset srcOffset, muf_usize size);
        void (* updateBuffer)(MufBuffer dst, muf_offset offset, muf_usize size, muf_crawptr data);
        void (* fillBuffer)(MufBuffer dst, muf_offset offset, muf_usize size, muf_u32 data);
        void (* copyTexture)(MufTexture dst, MufTextureCopyPos dstPos, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size);
        void (* copyTextureToBuffer)(MufBuffer dst, muf_usize dstOffset, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size);
        void (* copyBufferToTexture)(MufTexture dst, MufTextureCopyPos dstPos, MufBuffer src, muf_offset srcOffset, MufExtent3i size);
        void (* generateMipmaps)(MufTexture texture);
        void (* updateTexture)(MufTexture texture, MufOffset3i offset, MufExtent3i size, muf_crawptr data);
        void (* setPrimitiveTopology)(MufPrimitiveTopology topology);
        void (* setPrimitiveRestartEnabled)(muf_bool enabled);
        void (* setViewport)(const MufViewport *viewport);
        void (* setScissor)(const MufScissor *scissor);
        void (* setLineWidth)(muf_f32 width);
        void (* setStencilWriteMask)(MufStencilFaceFlags face, muf_u32 mask);
        void (* setStencilCompareMask)(MufStencilFaceFlags face, muf_u32 mask);
        void (* setStencilRef)(MufStencilFaceFlags face, muf_u32 ref);
        void (* setBlendConstant)(MufRGBA rgba);
        void (* setClearColor)(MufRGBA rgba);
        void (* setClearDepth)(muf_f32 depth);
        void (* setClearStencil)(muf_u32 stencil);
        void (* bindPipeline)(MufPipeline pipeline);
        void (* bindVertexBuffers)(const MufBuffer *buffers, const muf_offset *offsets, muf_index firstBindingIndex, muf_usize bindingCount);
        void (* bindVertexBuffer)(MufBuffer buffer, muf_offset offset, muf_index bindingIndex);
        void (* bindIndexBuffer)(MufBuffer buffer, muf_offset offset);
        void (* beginRenderPass)(MufRenderPass renderPass);
        void (* endRenderPass)();
        void (* bindResourceHeap)(MufResourceHeap resourceHeap);
        void (* draw)(muf_index firstIndex, muf_usize count);
        void (* drawIndexed)(muf_index firstIndex, muf_usize count);
    } cmd;
} MufRenderBackendApi;

typedef struct MufRenderBackendRegistry_s {
    const muf_char *name;
    void (*init)();
    void (*finish)();
    MufRenderBackendApi api;
} MufRenderBackendRegistry;

MufRenderBackend mufGetDefaultRenderBackend();
MufRenderBackend mufGetRenderBackend(const muf_char *name);

void mufRegisterRenderBackend(const MufRenderBackendRegistry *registry);
void mufSetDefaultRenderBackend(MufRenderBackend backend);

#define MUF_RENDER_BACKEND_DECLARE(Name) extern MufRenderBackend _muf_render_backend_##Name[1];
#define MUF_RENDER_BACKEND(Name) _muf_render_backend_##Name

const muf_char *mufRenderBackendGetName(MufRenderBackend backend);
MufRenderBackendApi *mufRenderBackendGetApi(MufRenderBackend backend);

#endif