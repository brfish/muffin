#include "muffin_platform/dlib.h"

#include <windows.h>

MufHandle mufDynlibLoad(const muf_char *path) {
    HMODULE lib = LoadLibrary(path);
    return mufCreateHandle(ptr, lib);
}

void mufDynlibClose(MufHandle handle) {
    HMODULE lib = (HMODULE) mufHandleCastRawptr(handle);
    FreeLibrary(lib);
}

void mufDynlibAddSearchPath(const muf_char *path) {
    AddDllDirectory(path);
}

MufProcAddress mufDynlibGetProcess(MufHandle lib, const muf_char *procName) {
    return (MufProcAddress) GetProcAddress((HMODULE) lib._ptr, procName);
}