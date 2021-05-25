#ifndef _MUFFIN_RENDER_FRAMEBUFFER_H_
#define _MUFFIN_RENDER_FRAMEBUFFER_H_

#include "muffin_core/common.h"

typedef enum MufAttachment_e {
    MUF_ATTACHMENT_COLOR,
    MUF_ATTACHMENT_DEPTH,
    MUF_ATTACHMENT_STENCIL,
    MUF_ATTACHMENT_DEPTH_STENCIL,

} MufAttachment;

typedef struct MufFrameBufferCreateInfo_s {
    MufAttachment attachment;
} MufFrameBufferCreateInfo;

typedef MufHandle MufFrameBuffer;

MufFrameBuffer mufCreateFrameBuffer(const MufFrameBufferCreateInfo *info);
void mufDestroyFrameBuffer(MufFrameBuffer framebuffer);

#endif