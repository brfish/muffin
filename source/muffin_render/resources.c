#include "muffin_render/resources.h"

#include "internal/backend_manager.h"

extern _MufRenderBackendManager *_mufRenderBackendManager;

MufBuffer mufCreateBuffer(const MufBufferCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createBuffer, info);
}

void mufDestroyBuffer(MufBuffer buffer) {
    _MUF_BACKEND_CHECK_CALL(destroyBuffer, buffer);
}

muf_rawptr mufMapBuffer(MufBuffer buffer, muf_offset offset, muf_usize size) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(mapBuffer, buffer, offset, size);
}

void mufUnmapBuffer(MufBuffer buffer) {
    _MUF_BACKEND_CHECK_CALL(unmapBuffer, buffer);
}

MufTexture mufCreateTexture(const MufTextureCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createTexture, info);
}

void mufDestroyTexture(MufTexture texture) {
    _MUF_BACKEND_CHECK_CALL(destroyTexture, texture);
}

MufSampler mufCreateSampler(const MufSamplerCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createSampler, info);
}

void mufDestroySampler(MufSampler sampler) {
    _MUF_BACKEND_CHECK_CALL(destroySampler, sampler);
}

MufShader mufCreateShader(const MufShaderCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createShader, info);
}

void mufDestroyShader(MufShader shader) {
    _MUF_BACKEND_CHECK_CALL(destroyShader, shader);
}

MufShaderProgram mufCreateShaderProgram(const MufShaderProgramCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createShaderProgram, info);
}

void mufDestroyShaderProgram(MufShaderProgram program) {
    _MUF_BACKEND_CHECK_CALL(destroyShaderProgram, program);
}

MufFramebuffer mufCreateFramebuffer(const MufFramebufferCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createFramebuffer, info);
}

void mufDestroyFramebuffer(MufFramebuffer framebuffer) {
    _MUF_BACKEND_CHECK_CALL(destroyFramebuffer, framebuffer);
}

MufRenderPass mufCreateRenderPass(const MufRenderPassCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createRenderPass, info);
}

void mufDestroyRenderPass(MufRenderPass renderPass) {
    _MUF_BACKEND_CHECK_CALL(destroyRenderPass, renderPass);
}

MufResourceHeap mufCreateResourceHeap(const MufResourceHeapCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createResourceHeap, info);
}

void mufDestroyResourceHeap(MufResourceHeap resourceHeap) {
    _MUF_BACKEND_CHECK_CALL(destroyResourceHeap, resourceHeap);
}

#undef _MUF_RESOURCE_CREATE_CALL