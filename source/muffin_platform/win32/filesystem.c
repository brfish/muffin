#include "muffin_platform/filesystem.h"

#include <windows.h>

#include "muffin_core/string.h"
#include "muffin_core/memory.h"

MufFile *mufCreateFile(const muf_char *path, MufFileOpenMode openMode) {
    MufFile *file = mufAlloc(MufFile, 1);
    file->openMode = openMode;
    file->path = path;

    muf_wchar utf16Path[256];
    mufUtf8ToUtf16(utf16Path, 256, path, mufCStrLength(path));

    HANDLE handle = NULL;
    if (openMode == MUF_FILE_OPEN_READ) {
        handle = CreateFileW(utf16Path, GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    } else if (openMode == MUF_FILE_OPEN_WRITE) {
        handle = CreateFileW(utf16Path, GENERIC_WRITE, 0, NULL, CREATE_NEW, 
            FILE_ATTRIBUTE_NORMAL, NULL);
    }

    if (handle == INVALID_HANDLE_VALUE) {
        file->handle._ptr = NULL;
    } else {
        file->handle._ptr = handle;
    }

    return file;
}

void mufDestroyFile(MufFile *file) {
    CloseHandle(file->handle._ptr);
    mufFree(file);
}

muf_bool mufFileIsOpened(const MufFile *file) {
    return file->handle._ptr != NULL;
}

muf_index mufFileTell(MufFile *file) {
#if MUF_SIZEOF_USIZE == 4
    DWORD lowPart = 0;
    lowPart = SetFilePointer(file->handle._ptr, 0, NULL, FILE_CURRENT);
    return (muf_index) lowPart;
#else
    LARGE_INTEGER moveDistance = { 0 };
    LARGE_INTEGER newFilePointer;
    SetFilePointerEx(file->handle._ptr, moveDistance, &newFilePointer, FILE_CURRENT);
    return (muf_index) newFilePointer.QuadPart;
#endif
}

muf_usize mufFileGetSize(MufFile *file) {
#if MUF_SIZEOF_USIZE == 4
    DWORD lowPart = 0;
    lowPart = GetFileSize(file->handle._ptr, NULL);
    return (muf_index) lowPart;
#else
    LARGE_INTEGER size;
    GetFileSizeEx(file->handle._ptr, &size);
    return (muf_usize) size.QuadPart;
#endif
}

const muf_char *mufFileGetPath(const MufFile *file) {
    return file->path;
}

void mufFileOpen(MufFile *file, const muf_char *path, MufFileOpenMode openMode) {

}

void mufFileClose(MufFile *file) {
    CloseHandle(file->handle._ptr);
    file->handle._ptr = NULL;
}

void mufFileSeek(MufFile *file, muf_offset offset, MufFileSeekMode seekMode) {
    const DWORD seekModes[] = { FILE_BEGIN, FILE_END, FILE_CURRENT };
#if MUF_SIZEOF_USIZE == 4
    SetFilePointer(file->handle._ptr, (LONG) offset, NULL, seekModes[seekMode]);
#else
    LARGE_INTEGER moveDistance, newFilePointer;
    moveDistance.QuadPart = (LONGLONG) offset;
    SetFilePointerEx(file->handle._ptr, moveDistance, &newFilePointer, seekModes[(muf_index) seekMode]);
#endif
}

muf_usize mufFileRead(MufFile *file, muf_usize size, muf_rawptr dataOut) {
#if MUF_SIZEOF_USIZE == 4
#elif MUF_SIZEOF_USIZE == 8
    if (size >= (muf_usize) MUF_U32_MAX) {
        muf_usize readSize = 0;
        DWORD partSize = 0;
        ReadFile(file->handle._ptr, dataOut, MUF_U32_MAX, &partSize, NULL);
        readSize += partSize;
        ReadFile(file->handle._ptr, (muf_byte *) dataOut + MUF_U32_MAX, size - MUF_U32_MAX, &partSize, NULL);
        readSize += partSize;
    } else {
        
    }
#else
#   error FUCK
#endif
    DWORD readSize = 0;
    ReadFile(file->handle._ptr, dataOut, (DWORD) size, &readSize, NULL);
    return (muf_usize) readSize;
}

muf_usize mufFileWrite(MufFile *file, const muf_rawptr data, muf_usize size) {
    DWORD writeSize = 0;
    WriteFile(file->handle._ptr, data, (DWORD) size, &writeSize, NULL);
    return (muf_usize) writeSize;
}

muf_usize mufFileTruncate(MufFile *file, muf_usize size) {

}

void mufFileFlush(MufFile *file) {
    FlushFileBuffers(file->handle._ptr);
}

void mufCopyFile(const muf_char *oldPath, const muf_char *newPath) {
    muf_wchar utf16OldPath[256];
    muf_wchar utf16NewPath[256];
    mufUtf8ToUtf16(utf16OldPath, 256, oldPath, mufCStrLength(oldPath));
    mufUtf8ToUtf16(utf16NewPath, 256, newPath, mufCStrLength(newPath));
    CopyFileW(utf16OldPath, utf16NewPath, 0);
}

void mufRemoveFile(const muf_char *filePath) {

}