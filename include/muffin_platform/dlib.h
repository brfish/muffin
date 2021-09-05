#ifndef _MUFFIN_PLATFORM_DYN_H_
#define _MUFFIN_PLATFORM_DYN_H_

#include "muffin_core/common.h"

typedef void(*MufProcAddress)(void);

MufHandle mufDynlibLoad(const muf_char *path);
void mufDynlibClose(MufHandle lib);
void mufDynlibAddSearchPath(const muf_char *path);
MufProcAddress mufDynlibGetProcAddress(MufHandle lib, const muf_char *procName);

#endif