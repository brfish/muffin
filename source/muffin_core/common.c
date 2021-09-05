#include "muffin_core/common.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void _mufAssertFormatPrint(const muf_char *filename, muf_index line,
    const muf_char *expression, const muf_char *format, ...) {
    fprintf(stderr, "Source: %s:%" PRIindex "\n", filename, line);

    if (expression) {
        fprintf(stderr, "Expected: `%s`\n", expression);
    }
    
    fprintf(stderr, "Message: ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

muf_bool mufEqual_i8(muf_crawptr a, muf_crawptr b) {
    const muf_i8 *x = (const muf_i8 *) a;
    const muf_i8 *y = (const muf_i8 *) b;
    return *x == *y;
}

muf_bool mufEqual_i16(muf_crawptr a, muf_crawptr b) {
    const muf_i16 *x = (const muf_i16 *) a;
    const muf_i16 *y = (const muf_i16 *) b;
    return *x == *y;
}

muf_bool mufEqual_i32(muf_crawptr a, muf_crawptr b) {
    const muf_i32 *x = (const muf_i32 *) a;
    const muf_i32 *y = (const muf_i32 *) b;
    return *x == *y;
}

muf_bool mufEqual_i64(muf_crawptr a, muf_crawptr b) {
    const muf_i64 *x = (const muf_i64 *) a;
    const muf_i64 *y = (const muf_i64 *) b;
    return *x == *y;
}

muf_bool mufEqual_u8(muf_crawptr a, muf_crawptr b) {
    const muf_u8 *x = (const muf_u8 *) a;
    const muf_u8 *y = (const muf_u8 *) b;
    return *x == *y;
}

muf_bool mufEqual_u16(muf_crawptr a, muf_crawptr b) {
    const muf_u16 *x = (const muf_u16 *) a;
    const muf_u16 *y = (const muf_u16 *) b;
    return *x == *y;
}

muf_bool mufEqual_u32(muf_crawptr a, muf_crawptr b) {
    const muf_u32 *x = (const muf_u32 *) a;
    const muf_u32 *y = (const muf_u32 *) b;
    return *x == *y;
}

muf_bool mufEqual_u64(muf_crawptr a, muf_crawptr b) {
    const muf_u64 *x = (const muf_u64 *) a;
    const muf_u64 *y = (const muf_u64 *) b;
    return *x == *y;
}

muf_bool mufEqual_str(muf_crawptr a, muf_crawptr b) {
    const muf_char *x = (const muf_char *) a;
    const muf_char *y = (const muf_char *) b;
    return x == y ? MUF_TRUE : strcmp(x, y) == 0;
}

muf_bool mufEqual_ptr(muf_crawptr a, muf_crawptr b) {
    return a == b;
}

void mufPrintln(const muf_char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    putchar('\n');
    va_end(args);
}

void mufVersionToString(MufVersion version, muf_char *strBuf) {
    muf_u32 major = mufVersionGetMajor(version);
    muf_u32 minor = mufVersionGetMinor(version);
    muf_u32 patch = mufVersionGetPatch(version);
    sprintf(strBuf, "%d.%d.%d",  major, minor, patch);
}