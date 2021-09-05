#include "muffin_platform/time.h"

#include <windows.h>

muf_u64 mufPlatformGetTime() {
    LARGE_INTEGER time;
    WINBOOL result = QueryPerformanceCounter(&time);
    MUF_ASSERT(result);
    return (muf_u64) time.QuadPart;
}

void mufPlatformSleep(muf_u32 milliseconds) {
    Sleep(milliseconds);
}