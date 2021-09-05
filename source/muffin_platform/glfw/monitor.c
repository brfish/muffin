#include "muffin_platform/monitor.h"

#include "glfw/glfw3.h"

#include "muffin_core/memory.h"

muf_usize mufMonitorGetAll(MufMonitor **monitors) {
    muf_i32 count = 0;
    GLFWmonitor **result = glfwGetMonitors(&count);
    MUF_ASSERT(count != 0);
    for (muf_index i = 0; i < count; ++i) {
        monitors[i]->handle = mufMakeHandle(MufHandle, ptr, result[i]);
    }
    return (muf_usize) count;
}

muf_usize mufMonitorGetCount() {
    muf_i32 count = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    MUF_UNUSED(monitors);
    return count;
}

MufMonitor *mufMonitorGetPrimary() {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    MufMonitor *wrapper = mufAlloc(MufMonitor, 1);
    wrapper->handle = mufMakeHandle(MufHandle, ptr, monitor);
    return wrapper;
}

MufOffset2i mufMonitorGetPosition(const MufMonitor *monitor) {
    muf_i32 x, y;
    glfwGetMonitorPos((GLFWmonitor *) monitor->handle._ptr, &x, &y);
    return (MufOffset2i) { x, y };
}

MufRect2i mufMonitorGetWorkArea(const MufMonitor *monitor) {
    muf_i32 x, y, w, h;
    glfwGetMonitorWorkarea((GLFWmonitor *) monitor->handle._ptr, &x, &y, &w, &h);
    return (MufRect2i) { x, y, w, h };
}

MufExtent2i mufMonitorGetPhysicalSize(const MufMonitor *monitor) {
    muf_i32 w, h;
    glfwGetMonitorPhysicalSize(monitor->handle._ptr, &w, &h);
    return (MufExtent2i) { w, h };
}

muf_u32 mufMonitorGetPhysicalWidth(const MufMonitor *monitor) {
    muf_i32 w;
    glfwGetMonitorPhysicalSize(monitor->handle._ptr, &w, NULL);
    return (muf_u32) w;
}

muf_u32 mufMonitorGetPhysicalHeight(const MufMonitor *monitor) {
    muf_i32 h;
    glfwGetMonitorPhysicalSize(monitor->handle._ptr, NULL, &h);
    return h;
}

MufVec2 mufMonitorGetDPIScale(const MufMonitor *monitor) {
    muf_f32 x, y;
    glfwGetMonitorContentScale(monitor->handle._ptr, &x, &y);
    return mufCreateVec2(x, y);
}

const muf_char *mufMonitorGetName(const MufMonitor *monitor) {
    return glfwGetMonitorName(monitor->handle._ptr);
}

void mufMonitorSetGamma(MufMonitor *monitor, float gamma) {
    glfwSetGamma(monitor->handle._ptr, gamma);
}