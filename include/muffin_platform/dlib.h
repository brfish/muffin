#ifndef _MUFFIN_PLATFORM_DLIB_H_
#define _MUFFIN_PLATFORM_DLIB_H_

#include "muffin_core/common.h"

MUF_HANDLE_DEF(MufLib);

typedef void(*MufProcAddress)(void);

MufLib mufDynlibLoad(const muf_char *path);
void mufDynlibClose(MufLib lib);
void mufDynlibAddSearchPath(const muf_char *path);
MufProcAddress mufDynlibGetProcAddress(MufLib lib, const muf_char *procName);

#endif