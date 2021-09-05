#ifndef _MUFFIN_CORE_STRING_H_
#define _MUFFIN_CORE_STRING_H_

#include <string.h>

#include "muffin_core/common.h"

#define mufCStrLength(Str) strlen(Str)
#define mufCStrCompare(Str1, Str2) strcmp(Str1, Str2)
#define mufCStrClone(Str) strdup(Str)
#define mufCStrCopy(Dst, Src) strcpy(Dst, Src)
#define mufCStrCopyN(Dst, Src, Count) strncpy(Dst, Src, Count)
#define mufCStrConcat(Dst, Src) strcat(Dst, Src)

typedef enum MufUtf8ResultCode_e {
    MUF_UTF8_SUCCESS,
    MUF_UTF8_TOO_SMALL,
    MUF_UTF8_INVALID
} MufUtf8ResultCode;

MUF_API muf_usize mufUtf8Length(const muf_char *str);
MUF_API muf_bool mufUtf8IsValid(const muf_char *str);
MUF_API muf_result mufUtf8ToUtf16(muf_wchar *dstUtf16, muf_usize dstMaxLength, const muf_char *srcUtf8, muf_usize srcLength);
MUF_API muf_result mufUtf16ToUtf8(muf_char *dstUtf8, muf_usize dstMaxLength, const muf_wchar *srcUtf16, muf_usize srcLength);

typedef struct MufStr_s {
    muf_usize capacity;
    muf_usize size;
} MufStr;

MufStr *mufCreateStr();

void mufDestroyStr(MufStr *str);

MufStr mufCloneStr(const MufStr *str);

muf_char *mufStrToCStr(const MufStr *str);

void mufStrConcat();

void mufStrConcatCStr();

#endif