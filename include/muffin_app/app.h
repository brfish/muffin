#ifndef _MUFFIN_APP_APP_H_
#define _MUFFIN_APP_APP_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"

const muf_char *mufAppGetTitle(void);
MufOffset2i mufAppGetPosition(void);
MufExtent2i mufAppGetSize(void);
muf_u32 mufAppGetWidth(void);
muf_u32 mufAppGetHeight(void);
MufPixels mufAppGetIcon(void);

muf_bool mufAppIsVisible(void);
muf_bool mufAppIsFullscreen(void);
muf_bool mufAppIsVsync(void);
muf_bool mufAppIsMinimized(void);

#if defined(MUF_PLATFORM_WIN32)
    typedef void * MufNativeHandle;
#endif

MufNativeHandle mufAppGetNativeHandle();

void mufAppSetTitle(const muf_char *title);
void mufAppSetPosition(MufOffset2i position);
void mufAppSetSize(MufExtent2i size);
void mufAppSetWidth(muf_u32 width);
void mufAppSetHeight(muf_u32 height);
void mufAppSetIcon(MufPixels pixels);
void mufAppSetVisible(muf_bool visible);
void mufAppSetFullscreen(muf_bool fullscreen);
void mufAppSetVsync(muf_bool vsync);
void mufAppMinimize(void);

void mufAppShow();

#endif