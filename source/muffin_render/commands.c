#include "muffin_render/commands.h"

#include "internal/backend_manager.h"

extern _MufRenderBackendManager *_mufRenderBackendManager;

#define _MUF_BACKEND_CMD_CALL(_func, ...) do { _MUF_CHECK_BACKEND(); _mufRenderBackendManager->defaultBackend->api.cmd._func(__VA_ARGS__); } while (0)

void mufCmdCopyBuffer(MufBuffer dst, muf_offset dstOffset, MufBuffer src, muf_offset srcOffset, muf_usize size) {
    _MUF_BACKEND_CMD_CALL(copyBuffer, dst, dstOffset, src, srcOffset, size);
}

void mufCmdUpdateBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_crawptr data) {
    _MUF_BACKEND_CMD_CALL(updateBuffer, dst, offset, size, data);
}

void mufCmdFillBuffer(MufBuffer dst, muf_offset offset, muf_usize size, muf_u32 data) {
    _MUF_BACKEND_CMD_CALL(fillBuffer, dst, offset, size, data);
}

void mufCmdCopyTexture(MufTexture dst, MufTextureCopyPos dstPos, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size) {
    _MUF_BACKEND_CMD_CALL(copyTexture, dst, dstPos, src, srcPos, size);
}

void mufCmdCopyTextureToBuffer(MufBuffer dst, muf_usize dstOffset, MufTexture src, MufTextureCopyPos srcPos, MufExtent3i size) {
    _MUF_BACKEND_CMD_CALL(copyTextureToBuffer, dst, dstOffset, src, srcPos, size);
}

void mufCmdCopyBufferToTexture(MufTexture dst, MufTextureCopyPos dstPos, MufBuffer src, muf_offset srcOffset, MufExtent3i size) {
    _MUF_BACKEND_CMD_CALL(copyBufferToTexture, dst, dstPos, src, srcOffset, size);
}

void mufCmdGenerateMipmaps(MufTexture texture) {
    _MUF_BACKEND_CMD_CALL(generateMipmaps, texture);
}

void mufCmdUpdateTexture(MufTexture texture, MufOffset3i offset, MufExtent3i size, muf_crawptr data) {
    _MUF_BACKEND_CMD_CALL(updateTexture, texture, offset, size, data);
}

void mufCmdSetPrimitiveTopology(MufPrimitiveTopology topology) {
    _MUF_BACKEND_CMD_CALL(setPrimitiveTopology, topology);
}

void mufCmdSetPrimitiveRestartEnabled(muf_bool enabled) {
    _MUF_BACKEND_CMD_CALL(setPrimitiveRestartEnabled, enabled);
}

void mufCmdSetViewport(const MufViewport *viewport) {
    _MUF_BACKEND_CMD_CALL(setViewport, viewport);
}

void mufCmdSetScissor(const MufScissor *scissor) {
    _MUF_BACKEND_CMD_CALL(setScissor, scissor);
}

void mufCmdSetLineWidth(muf_f32 width) {
    _MUF_BACKEND_CMD_CALL(setLineWidth, width);
}

void mufCmdSetStencilWriteMask(MufStencilFaceFlags face, muf_u32 mask) {
    _MUF_BACKEND_CMD_CALL(setStencilWriteMask, face, mask);
}

void mufCmdSetStencilCompareMask(MufStencilFaceFlags face, muf_u32 mask) {
    _MUF_BACKEND_CMD_CALL(setStencilCompareMask, face, mask);
}

void mufCmdSetStencilRef(MufStencilFaceFlags face, muf_u32 ref) {
    _MUF_BACKEND_CMD_CALL(setStencilRef, face, ref);
}

void mufCmdSetBlendConstant(MufRGBA rgba) {
    _MUF_BACKEND_CMD_CALL(setBlendConstant, rgba);
}

void mufCmdSetClearColor(MufRGBA rgba) {
    _MUF_BACKEND_CMD_CALL(setClearColor, rgba);
}

void mufCmdSetClearDepth(muf_f32 depth) {
    _MUF_BACKEND_CMD_CALL(setClearDepth, depth);
}

void mufCmdSetClearStencil(muf_u32 stencil) {
    _MUF_BACKEND_CMD_CALL(setClearStencil, stencil);
}

void mufCmdBindPipeline(MufPipeline pipeline) {
    _MUF_BACKEND_CMD_CALL(bindPipeline, pipeline);
}

void mufCmdBindVertexBuffers(const MufBuffer *buffers, const muf_offset *offsets, muf_index firstBindingIndex, muf_usize bindingCount) {
    _MUF_BACKEND_CMD_CALL(bindVertexBuffers, buffers, offsets, firstBindingIndex, bindingCount);
}

void mufCmdBindVertexBuffer(MufBuffer buffer, muf_offset offset, muf_index bindingIndex) {
    _MUF_BACKEND_CMD_CALL(bindVertexBuffer, buffer, offset, bindingIndex);
}

void mufCmdBindIndexBuffer(MufBuffer buffer, muf_offset offset) {
    _MUF_BACKEND_CMD_CALL(bindIndexBuffer, buffer, offset);
}

void mufCmdBindResourceHeap(MufResourceHeap resourceHeap) {
    _MUF_BACKEND_CMD_CALL(bindResourceHeap, resourceHeap);
}

void mufCmdBeginRenderPass(MufRenderPass renderPass) {
    _MUF_BACKEND_CMD_CALL(beginRenderPass, renderPass);
}

void mufCmdEndRenderPass() {
    _MUF_BACKEND_CMD_CALL(endRenderPass);
}

void mufCmdDraw(muf_index firstIndex, muf_usize count) {
    _MUF_BACKEND_CMD_CALL(draw, firstIndex, count);
}

void mufCmdDrawIndexed(muf_index firstIndex, muf_usize count) {
    _MUF_BACKEND_CMD_CALL(drawIndexed, firstIndex, count);
}

void mufCmdBlit();

#undef _MUF_BACKEND_CMD_CALL