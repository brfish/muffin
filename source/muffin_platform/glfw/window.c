#include "muffin_platform/window.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "muffin_core/memory.h"
#include "muffin_core/string.h"

typedef struct _internal_MufWindow_s {
    MufHandle           handle;
    MufMonitor          monitor;
    muf_bool            vsync;
    muf_char            *title;
    MufPixels           icon;
    MufWindowSizeLimits limits;
} _internal_MufWindow;

#define _WINDOWPTR(_windowptr) ((GLFWwindow *) _windowptr->handle._ptr)

MufWindow *mufCreateWindow(muf_i32 x, muf_i32 y, muf_usize width, muf_usize height, const muf_char *title) {
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    GLFWmonitor *monitor = glfwGetWindowMonitor(window);
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, x, y);
    _internal_MufWindow *win = mufAlloc(_internal_MufWindow, 1);
    win->handle = mufMakeHandle(MufHandle, ptr, window);
    win->monitor.handle = mufMakeHandle(MufHandle, ptr, monitor);
    win->vsync = MUF_FALSE;
    win->title = mufCStrClone(title);
    win->icon = (MufPixels) { 0 };
    win->limits.minWidth = win->limits.maxWidth = GLFW_DONT_CARE;
    win->limits.minHeight = win->limits.maxHeight = GLFW_DONT_CARE;
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        puts("NO!!!");
    return (MufWindow *) win;
}

void mufDestroyWindow(MufWindow *window) {
    glfwDestroyWindow(window->handle._ptr);
    _internal_MufWindow *win = (_internal_MufWindow *) window;
    mufFree(win->title);
    mufFree(win);
}

muf_bool mufWindowShouldClose(const MufWindow *window) {
    return glfwWindowShouldClose(_WINDOWPTR(window));
}

MufExtent2i mufWindowGetSize(const MufWindow *window) {
    muf_i32 w, h;
    glfwGetWindowSize(_WINDOWPTR(window), &w, &h);
    return (MufExtent2i) { (muf_u32) w, (muf_u32) h };
}

muf_u32 mufWindowGetWidth(const MufWindow *window) {
    muf_i32 w;
    glfwGetWindowSize(_WINDOWPTR(window), &w, NULL);
    return (muf_u32) w;
}

muf_u32 mufWindowGetHeight(const MufWindow *window) {
    muf_i32 h;
    glfwGetWindowSize(_WINDOWPTR(window), NULL, &h);
    return (muf_u32) h;
}

const MufWindowSizeLimits *mufWindowGetSizeLimit(const MufWindow *window) {
    return &((_internal_MufWindow *) window)->limits;
}

MufOffset2i mufWindowGetPosition(const MufWindow *window) {
    muf_i32 x, y;
    glfwGetWindowPos(_WINDOWPTR(window), &x, &y);
    return (MufOffset2i) { x, y };
}

muf_i32 mufWindowGetX(const MufWindow *window) {
    muf_i32 x;
    glfwGetWindowPos(_WINDOWPTR(window), &x, NULL);
    return x;
}

muf_i32 mufWindowGetY(const MufWindow *window) {
    muf_i32 y;
    glfwGetWindowPos(_WINDOWPTR(window), NULL, &y);
    return y;
}

const muf_char *mufWindowGetTitle(const MufWindow *window) {
    return ((const _internal_MufWindow *) window)->title;
}

MufPixels mufWindowGetIcon(const MufWindow *window) {
    return ((const _internal_MufWindow *) window)->icon;
}

muf_bool mufWindowIsVsync(const MufWindow *window) {
    return ((const _internal_MufWindow *) window)->vsync;
}

muf_bool mufWindowIsFullscreen(const MufWindow *window) {
    return MUF_FALSE;
}

muf_bool mufWindowIsVisible(const MufWindow *window) {
    return (muf_bool) glfwGetWindowAttrib(_WINDOWPTR(window), GLFW_VISIBLE);   
}

const MufMonitor *mufWindowGetMonitor(const MufWindow *window) {
    return &((const _internal_MufWindow *) window)->monitor;
}

MufRect2pi mufWindowGetFrameSize(const MufWindow *window) {
    muf_i32 l, t, r, b;
    glfwGetWindowFrameSize(_WINDOWPTR(window), &l, &t, &r, &b);
    return (MufRect2pi) { l, t, r, b };
}

muf_bool mufWindowIsMaximized(const MufWindow *window) {
    return (muf_bool) glfwGetWindowAttrib(_WINDOWPTR(window), GLFW_MAXIMIZED);
}

muf_bool mufWindowIsMinimized(const MufWindow *window) {
    return (muf_bool) !glfwGetWindowAttrib(_WINDOWPTR(window), GLFW_VISIBLE);
}

void mufWindowSetSize(MufWindow *window, MufExtent2i size) {
    glfwSetWindowSize(_WINDOWPTR(window), (muf_i32) size.width, (muf_i32) size.height);
}

void mufWindowSetWidth(MufWindow *window, muf_u32 width) {
    muf_u32 height = mufWindowGetHeight(window);
    glfwSetWindowSize(_WINDOWPTR(window), width, height);
}

void mufWindowSetHeight(MufWindow *window, muf_u32 height) {
    muf_u32 width = mufWindowGetWidth(window);
    glfwSetWindowSize(_WINDOWPTR(window), width, height);
}

void mufWindowSetSizeLimits(MufWindow *window, const MufWindowSizeLimits *limits) {
    _internal_MufWindow *win = (_internal_MufWindow *) window;
    glfwSetWindowSizeLimits(_WINDOWPTR(window), limits->minWidth, limits->minHeight, 
        limits->maxWidth, limits->maxHeight);
    mufMemCopy(&win->limits, limits, MufWindowSizeLimits, 1);
}

void mufWindowSetPosition(MufWindow *window, MufOffset2i position) {
    glfwSetWindowPos(_WINDOWPTR(window), position.x, position.y);
}

void mufWindowSetX(MufWindow *window, muf_i32 x) {
    muf_u32 y = mufWindowGetY(window);
    glfwSetWindowPos(_WINDOWPTR(window), x, y);
}

void mufWindowSetY(MufWindow *window, muf_i32 y) {
    muf_u32 x = mufWindowGetX(window);
    glfwSetWindowPos(_WINDOWPTR(window), x, y);
}

void mufWindowSetTitle(MufWindow *window, const muf_char *title) {
    glfwSetWindowTitle(_WINDOWPTR(window), title);
}

void mufWindowSetIcon(MufWindow *window, const MufPixels *icon) {
    _internal_MufWindow *win = (_internal_MufWindow *) window;
    GLFWimage image;
    image.width = icon->width;
    image.height = icon->height;
    image.pixels = icon->pixels;
    mufMemCopy(&win->icon, icon, MufPixels, 1);
    glfwSetWindowIcon(_WINDOWPTR(window), 1, &image);
}

void mufWindowSetVsync(MufWindow *window, muf_bool vsync) {
    glfwSwapInterval(vsync);
}

void mufWindowSetFullscreen(MufWindow *window, muf_bool fullscreen);

void mufWindowSetVisible(MufWindow *window, muf_bool visible) {
    glfwSetWindowAttrib(_WINDOWPTR(window), GLFW_VISIBLE, visible);
}

void mufWindowSetMonitor(MufWindow *window, MufMonitor *monitor, MufRect2i area) {
    _internal_MufWindow *win = (_internal_MufWindow *) window;
    glfwSetWindowMonitor(_WINDOWPTR(window), (GLFWmonitor *) monitor->handle._ptr, 
        area.x, area.y, area.width, area.height, GLFW_DONT_CARE);
    win->monitor = *monitor;
}

void mufWindowFocus(MufWindow *window) {
    glfwFocusWindow(_WINDOWPTR(window));
}

void mufWindowHide(MufWindow *window) {
    glfwHideWindow(_WINDOWPTR(window));
}

void mufWindowShow(MufWindow *window) {
    glfwShowWindow(_WINDOWPTR(window));
}

void mufWindowClose(MufWindow *window) {
    glfwSetWindowShouldClose(_WINDOWPTR(window), GLFW_TRUE);
}

void mufWindowMaximize(MufWindow *window) {
    glfwMaximizeWindow(_WINDOWPTR(window));
}

void mufWindowMinimize(MufWindow *window) {
    mufWindowHide(window);
}

void mufWindowSwapBuffers(MufWindow *window) {
    glfwSwapBuffers(_WINDOWPTR(window));
}

void mufWindowPollEvents(MufWindow *window) {
    glfwPollEvents();
}

void mufWindowSetCurrentContext(MufWindow *window) {
    glfwMakeContextCurrent(_WINDOWPTR(window));
}

#undef _WINDOWPTR