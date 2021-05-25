#ifndef _MUFFIN_RENDER_CONTEXT_H_
#define _MUFFIN_RENDER_CONTEXT_H_

#include "muffin_core/common.h"

typedef struct MufContext_s {
    muf_rawptr hwnd;
    muf_rawptr backBuffer;
    muf_rawptr backDepthBuffer;
    muf_rawptr backStencilBuffer;
} MufContext;

MufContext mufCreateContext();

void mufDestroyContext(MufContext *context);

#endif