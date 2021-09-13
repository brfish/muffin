#include "muffin_platform/dlib.h"

#include <windows.h>

#include "muffin_core/string.h"

MufLib mufDynlibLoad(const muf_char *path) {
    HMODULE lib = LoadLibrary(path);
    return mufMakeHandle(MufLib, ptr, lib);
}

void mufDynlibClose(MufLib handle) {
    HMODULE lib = (HMODULE) mufHandleCastRawptr(handle);
    FreeLibrary(lib);
}

void mufDynlibAddSearchPath(const muf_char *path) {
    muf_wchar pathBuf[256];
    mufUtf8ToUtf16(pathBuf, 256, path, mufCStrLength(path));   
    AddDllDirectory(pathBuf);
}

MufProcAddress mufDynlibGetProcess(MufLib lib, const muf_char *procName) {
    return (MufProcAddress) GetProcAddress((HMODULE) mufHandleCastRawptr(lib), procName);
}