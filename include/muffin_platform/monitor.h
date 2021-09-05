#ifndef _MUFFIN_PLATFORM_MONITOR_H_
#define _MUFFIN_PLATFORM_MONITOR_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"

typedef struct MufMonitor_s {
    MufHandle handle;
} MufMonitor;

typedef struct MufGammaCorrect_s {
    MufColor *gamma;
    muf_usize size;
} MufGammaCorrect;

MUF_API muf_usize mufMonitorGetAll(MufMonitor **monitors);
MUF_API muf_usize mufMonitorGetCount();
MUF_API MufMonitor *mufMonitorGetPrimary();
MUF_API MufOffset2i mufMonitorGetPosition(const MufMonitor *monitor);
MUF_API MufRect2i mufMonitorGetWorkArea(const MufMonitor *monitor);
MUF_API MufExtent2i mufMonitorGetPhysicalSize(const MufMonitor *monitor);
MUF_API muf_u32 mufMonitorGetPhysicalWidth(const MufMonitor *monitor);
MUF_API muf_u32 mufMonitorGetPhysicalHeight(const MufMonitor *monitor);
MUF_API MufVec2 mufMonitorGetDPIScale(const MufMonitor *monitor);
MUF_API const muf_char *mufMonitorGetName(const MufMonitor *monitor);
MUF_API void mufMonitorSetGamma(MufMonitor *monitor, float gamma);

#endif