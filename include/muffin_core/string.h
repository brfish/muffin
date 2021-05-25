#ifndef _MUFFIN_CORE_STRING_H_
#define _MUFFIN_CORE_STRING_H_

#include "muffin_core/common.h"

enum MufUTF8ResultCode {
    MUF_UTF8_SUCCESS,
    MUF_UTF8_TOO_SMALL,
    MUF_UTF8_INVALID
};

muf_usize mufUTF8Length(const muf_char *str);

muf_i32 mufConvertUTF8ToUTF16(const muf_char *src, muf_usize srcLength, muf_wchar *dst, muf_usize dstMaxSize);

void mufConvertUTF16ToUtf8(const muf_wchar *str, muf_char *resultOut);

typedef struct MufString_s {
    muf_usize capacity;
    muf_usize size;
} MufString;

#endif