#ifndef _MUFFIN_RENDER_GL_BACKEND_H_
#define _MUFFIN_RENDER_GL_BACKEND_H_

#include "glad/glad.h"

#include "muffin_render/buffer.h"
#include "muffin_render/format.h"
#include "muffin_render/framebuffer.h"
#include "muffin_render/pipeline.h"
#include "muffin_render/shader.h"
#include "muffin_render/texture.h"

MufBuffer mufGLCreateBuffer(const MufBufferCreateInfo *info);
void mufGLDestroyBuffer(MufBuffer buffer);
void mufGLBufferUpdate(MufBuffer buffer, muf_usize offset, const muf_rawptr data, muf_usize size);
void mufGLBufferClear(MufBuffer buffer, MufFormat internalForamt, MufFormat format, const muf_rawptr data);

MufTexture mufGLCreateTexture(const MufTextureCreateInfo *info);
void mufGLDestroyTexture(MufTexture texture);

MUF_API MufShader mufGLCreateShader(const MufShaderCreateInfo *info);
MUF_API void mufGLDestroyShader(MufShader shader);
MUF_API MufShaderProgram mufGLCreateShaderProgram(const MufShaderProgramCreateInfo *info);
MUF_API void mufGLDestroyShaderProgram(MufShaderProgram program);
void mufGLShaderGet(const muf_char *variantName, muf_rawptr dataOut);
void mufGLShaderSet(const muf_char *variantName, const muf_rawptr data);

MufPipeline mufGLCreatePipeline(const MufPipelineCreateInfo *info);
void mufGLDestroyPipeline(MufPipeline pipeline);
void mufGLBindPipeline(MufPipeline pipeline);



#endif