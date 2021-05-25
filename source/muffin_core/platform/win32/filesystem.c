#include "muffin_core/filesystem.h"

#include "muffin_core/string.h"

#include <windows.h>

MufFile *mufOpenFile(const muf_char *path, MufFileOpenMode openMode) {
    DWORD desiredAccess = 0;

    if (openMode & MUF_FILE_OPEN_READ)
        desiredAccess |= GENERIC_READ;
    if (openMode & MUF_FILE_OPEN_WRITE)
        desiredAccess |= GENERIC_WRITE;

}

void mufCloseFile(MufFile *file) {
    if (!file || !file->handle.asRawptr)
        return;
    CloseHandle(file->handle.asRawptr);
}

muf_bool mufFileIsOpened(const MufFile *file) {
    return file->handle.asRawptr != NULL;
}

muf_usize mufFileRead(MufFile *file, muf_usize size, muf_rawptr dataOut) {
    DWORD readCount = 0;
    ReadFile(file->handle.asRawptr, dataOut, (DWORD) size, &readCount, NULL);
    return (muf_usize) readCount;
}

muf_usize mufFileWrite(MufFile *file, const muf_rawptr data, muf_usize size) {
    DWORD writeCount = 0;
    WriteFile(file->handle.asRawptr, data, (DWORD) size, &writeCount, NULL);
    return (muf_usize) writeCount;
}

muf_index mufFileTell(MufFile *file) {
#if MUF_SIZEOF_USIZE == 4
    DWORD lowPart = 0;
    lowPart = SetFilePointer(file->handle.asRawptr, 0, NULL, FILE_CURRENT);
    return (muf_index) lowPart;
#else
    DWORD highPart = 0, lowPart = 0;
    lowPart = SetFilePointer(file->handle.asRawptr, 0, &highPart, FILE_CURRENT);
    return (muf_index) ((highPart << 32) | lowPart);
#endif
}

void mufFileSeek(MufFile *file, muf_offset offset, MufFileSeekMode seekMode) {
    DWORD seekModes[] = { FILE_BEGIN, FILE_END, FILE_CURRENT };
#if MUF_SIZEOF_USIZE == 4
    SetFilePointer(file->handle.asRawptr, (LONG) offset, NULL, seekModes[seekMode]);
#else
    if (offset < (1LL << 31)) {
        SetFilePointer(file->handle.asRawptr, (LONG) offset, NULL, seekModes[seekMode]);
    } else {
        LARGE_INTEGER v;
        v.QuadPart = offset;
        SetFilePointer(file->handle.asRawptr, v.LowPart, &v.HighPart, seekModes[seekMode]);
    }
#endif
}

muf_usize mufFileGetSize(MufFile *file) {
#if MUF_SIZEOF_USIZE == 4
    DWORD lowPart = 0;
    lowPart = GetFileSize(file->handle.asRawPtr, NULL);
    return (muf_index) lowPart;
#else
    DWORD highPart = 0, lowPart = 0;
    lowPart = GetFileSize(file->handle.asRawptr, &highPart);
    return (muf_index) ((highPart << 32) | lowPart);
#endif
}

void mufCopyFile(const muf_char *oldPath, const muf_char *newPath) {
    muf_wchar oldPathBuffer[128];
    muf_wchar newPathBuffer[128];
    mufConvertUTF8ToUTF16(oldPath, oldPathBuffer);
    mufConvertUTF8ToUTF16(newPath, newPathBuffer);
    CopyFileW(oldPathBuffer, newPathBuffer, 0);
}