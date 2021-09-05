#ifndef _MUFFIN_PLATFORM_TIME_H_
#define _MUFFIN_PLATFORM_TIME_H_

#include "muffin_core/common.h"

muf_u64 mufPlatformGetTime();
void mufPlatformSleep(muf_u32 milliseconds);

muf_u64 mufGetTimeStamp();

#endif