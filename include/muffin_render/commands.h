#ifndef _MUFFIN_RENDER_COMMANDS_H_
#define _MUFFIN_RENDER_COMMANDS_H_

#include "muffin_render/enums.h"
#include "muffin_render/resources.h"
#include "muffin_render/pipeline.h"

MUF_HANDLE_DEF(MufCommandBuffer);

MufCommandBuffer mufCreateCommandBuffer();
void mufDestroyCommandBuffer(MufCommandBuffer commandBuffer);

void mufCommandBufferBegin(MufCommandBuffer commandBuffer);
void mufCommandBufferEnd(MufCommandBuffer commandBuffer);

typedef struct MufTextureCopyPos_s {
    MufOffset3i offset;
    muf_u32     layerIndex;
    muf_u32     mipLevels;
} MufTextureCopyPos;

typedef struct MufTextureLocation_s {

} MufTextureLocation;

MUF_API void mufCmdCopyBuffer(MufBuffer dst, muf_offset dstOffset, MufBuffer src, muf_offset srcOffset, muf_usize size);
MUF_API void mufCmdUpdateBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_crawptr data);
MUF_API void mufCmdFillBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_u32 data);

MUF_API void mufCmdCopyTexture(MufTexture dst, MufTextureCopyPos dstPos, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size);
MUF_API void mufCmdCopyTextureToBuffer(MufBuffer dst, muf_usize dstOffset, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size);
MUF_API void mufCmdCopyBufferToTexture(MufTexture dst, MufTextureCopyPos dstPos, MufBuffer src, muf_offset srcOffset, MufExtent3i size);
MUF_API void mufCmdGenerateMipmaps(MufTexture texture);
MUF_API void mufCmdUpdateTexture(MufTexture texture, MufOffset3i offset, MufExtent3i size, muf_crawptr data);

MUF_API void mufCmdSetPrimitiveTopology(MufPrimitiveTopology topology);
MUF_API void mufCmdSetPrimitiveRestartEnabled(muf_bool enabled);
MUF_API void mufCmdSetViewport(const MufViewport *viewport);
MUF_API void mufCmdSetScissor(const MufScissor *scissor);
MUF_API void mufCmdSetLineWidth(muf_f32 width);
MUF_API void mufCmdSetStencilWriteMask(MufStencilFaceFlags face, muf_u32 mask);
MUF_API void mufCmdSetStencilCompareMask(MufStencilFaceFlags face, muf_u32 mask);
MUF_API void mufCmdSetStencilRef(MufStencilFaceFlags face, muf_u32 ref);
MUF_API void mufCmdSetBlendConstant(MufRGBA rgba);

MUF_API void mufCmdSetClearColor(MufRGBA rgba);
MUF_API void mufCmdSetClearDepth(muf_f32 depth);
MUF_API void mufCmdSetClearStencil(muf_u32 stencil);

MUF_API void mufCmdBindPipeline(MufPipeline pipeline);
MUF_API void mufCmdBindVertexBuffers(const MufBuffer *buffers, const muf_offset *offsets, muf_index firstBindingIndex, muf_usize bindingCount);
MUF_API void mufCmdBindVertexBuffer(MufBuffer buffer, muf_offset offset, muf_index bindingIndex);
MUF_API void mufCmdBindIndexBuffer(MufBuffer buffer, muf_offset offset);
MUF_API void mufCmdBindResourceHeap(MufResourceHeap resourceHeap);

MUF_API void mufCmdBeginRenderPass(MufRenderPass renderPass);
MUF_API void mufCmdEndRenderPass();

MUF_API void mufCmdDraw(muf_index firstIndex, muf_usize count);
MUF_API void mufCmdDrawIndexed(muf_index firstIndex, muf_usize count);
MUF_API void mufCmdBlit();

#endif
