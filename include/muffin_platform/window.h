#ifndef _MUFFIN_PLATFORM_WINDOW_H_
#define _MUFFIN_PLATFORM_WINDOW_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"    
#include "muffin_platform/monitor.h"

typedef struct MufWindow_s {
    MufHandle handle;
} MufWindow;

typedef struct MufWindowSizeLimits_s {
    muf_u32 minWidth;
    muf_u32 minHeight;
    muf_u32 maxWidth;
    muf_u32 maxHeight;
} MufWindowSizeLimits;

MUF_API MufWindow *mufCreateWindow(muf_i32 x, muf_i32 y, muf_usize width, muf_usize height, const muf_char *title);
MUF_API void mufDestroyWindow(MufWindow *window);
MUF_API muf_bool mufWindowShouldClose(const MufWindow *window);
MUF_API MufExtent2i mufWindowGetSize(const MufWindow *window);
MUF_API muf_u32 mufWindowGetWidth(const MufWindow *window);
MUF_API muf_u32 mufWindowGetHeight(const MufWindow *window);
MUF_API const MufWindowSizeLimits *mufWindowGetSizeLimits(const MufWindow *window);
MUF_API MufOffset2i mufWindowGetPosition(const MufWindow *window);
MUF_API muf_i32 mufWindowGetX(const MufWindow *window);
MUF_API muf_i32 mufWindowGetY(const MufWindow *window);
MUF_API const muf_char *mufWindowGetTitle(const MufWindow *window);
MUF_API MufPixels mufWindowGetIcon(const MufWindow *window);
MUF_API muf_bool mufWindowIsVsync(const MufWindow *window);
MUF_API muf_bool mufWindowIsFullscreen(const MufWindow *window);
MUF_API muf_bool mufWindowIsVisible(const MufWindow *window);
MUF_API const MufMonitor *mufWindowGetMonitor(const MufWindow *window);
MUF_API MufRect2pi mufWindowGetFrameSize(const MufWindow *window);
MUF_API muf_bool mufWindowIsMaximized(const MufWindow *window);
MUF_API muf_bool mufWindowIsMinimized(const MufWindow *window);

MUF_API void mufWindowSetSize(MufWindow *window, MufExtent2i size);
MUF_API void mufWindowSetWidth(MufWindow *window, muf_u32 width);
MUF_API void mufWindowSetHeight(MufWindow *window, muf_u32 height);
MUF_API void mufWindowSetSizeLimits(MufWindow *window, const MufWindowSizeLimits *limits);
MUF_API void mufWindowSetPosition(MufWindow *window, MufOffset2i position);
MUF_API void mufWindowSetX(MufWindow *window, muf_i32 x);
MUF_API void mufWindowSetY(MufWindow *window, muf_i32 y);
MUF_API void mufWindowSetTitle(MufWindow *window, const muf_char *title);
MUF_API void mufWindowSetIcon(MufWindow *window, const MufPixels *icon);
MUF_API void mufWindowSetVsync(MufWindow *window, muf_bool vsync);
MUF_API void mufWindowSetFullscreen(MufWindow *window, muf_bool fullscreen);
MUF_API void mufWindowSetVisible(MufWindow *window, muf_bool visible);
MUF_API void mufWindowSetMonitor(MufWindow *window, MufMonitor *monitor, MufRect2i area);

MUF_API void mufWindowFocus(MufWindow *window);
MUF_API void mufWindowHide(MufWindow *window);
MUF_API void mufWindowShow(MufWindow *window);
MUF_API void mufWindowClose(MufWindow *window);
MUF_API void mufWindowMaximize(MufWindow *window);
MUF_API void mufWindowMinimize(MufWindow *window);

MUF_API void mufWindowSwapBuffers(MufWindow *window);
MUF_API void mufWindowPollEvents(MufWindow *window);
MUF_API void mufWindowSetCurrentContext(MufWindow *window);

#endif