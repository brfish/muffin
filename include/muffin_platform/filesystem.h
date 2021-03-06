#ifndef _MUFFIN_PLATFORM_FILESYSTEM_H_
#define _MUFFIN_PLATFORM_FILESYSTEM_H_

#include "muffin_core/common.h"

typedef enum MufFileOpenMode_e {
    MUF_FILE_OPEN_READ,
    MUF_FILE_OPEN_WRITE,
    MUF_FILE_OPEN_APPEND,
    MUF_FILE_OPEN_TRUNC
} MufFileOpenMode;

typedef enum MufFileSeekMode_e {
    MUF_FILE_SEEK_BEG,
    MUF_FILE_SEEK_END,
    MUF_FILE_SEEK_CUR
} MufFileSeekMode;

typedef struct MufFile_s {
    MufHandle       handle;
    MufFileOpenMode openMode;
    const muf_char  *path;
} MufFile;

MUF_API MufFile *mufCreateFile(const muf_char *path, MufFileOpenMode openMode);
MUF_API void mufDestroyFile(MufFile *file);

MUF_API muf_bool mufFileIsOpened(const MufFile *file);
MUF_API muf_index mufFileTell(MufFile *file);
MUF_API muf_usize mufFileGetSize(MufFile *file);
MUF_API const muf_char *mufFileGetPath(const MufFile *file);

MUF_API void mufFileOpen(MufFile *file, const muf_char *path, MufFileOpenMode openMode);
MUF_API void mufFileClose(MufFile *file);
MUF_API void mufFileSeek(MufFile *file, muf_offset offset, MufFileSeekMode seekMode);
MUF_API muf_usize mufFileRead(MufFile *file, muf_usize size, muf_rawptr dataOut);
MUF_API muf_usize mufFileWrite(MufFile *file, const muf_rawptr data, muf_usize size);
MUF_API muf_usize mufFileTruncate(MufFile *file, muf_usize size);
MUF_API void mufFileFlush(MufFile *file);

MUF_API void mufGetCurrentDir();
MUF_API void mufGetHomeDir();

MUF_API muf_bool mufExistsFile(const muf_char *filePath);

MUF_API void mufNewFile(const muf_char *filePath);
MUF_API void mufNewDirectory(const muf_char *directoryPath);
MUF_API void mufCopyFile(const muf_char *oldPath, const muf_char *newPath);
MUF_API void mufCopyDirectory(const muf_char *oldPath, const muf_char *newPath);
MUF_API void mufMoveFile(const muf_char *oldPath, const muf_char *newPath);
MUF_API void mufMoveDirectory(const muf_char *oldPath, const muf_char *newPath);
MUF_API void mufRemoveFile(const muf_char *filePath);
MUF_API void mufRemoveDirectory(const muf_char *directoryPath);
MUF_API void mufRenameFile(const muf_char *filePath);
MUF_API void mufRenameDirectory(const muf_char *directoryPath);

#endif