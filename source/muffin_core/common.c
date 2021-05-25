#include "muffin_core/common.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void _mufAssertFormatPrint(const char *filename, muf_index line,
    const char *expression, const char *format, ...) {
    printf("Source: %s:%d\n", filename, line);
    printf("Expected: %s\n", expression);
    printf("Message: ");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    putchar('\n');
}

muf_bool mufEqual_i8(const muf_rawptr a, const muf_rawptr b) {
    const muf_i8 *x = (const muf_i8 *) a;
    const muf_i8 *y = (const muf_i8 *) b;
    return *x == *y;
}

muf_bool mufEqual_i16(const muf_rawptr a, const muf_rawptr b) {
    const muf_i16 *x = (const muf_i16 *) a;
    const muf_i16 *y = (const muf_i16 *) b;
    return *x == *y;
}

muf_bool mufEqual_i32(const muf_rawptr a, const muf_rawptr b) {
    const muf_i32 *x = (const muf_i32 *) a;
    const muf_i32 *y = (const muf_i32 *) b;
    return *x == *y;
}

muf_bool mufEqual_i64(const muf_rawptr a, const muf_rawptr b) {
    const muf_i64 *x = (const muf_i64 *) a;
    const muf_i64 *y = (const muf_i64 *) b;
    return *x == *y;
}

muf_bool mufEqual_u8(const muf_rawptr a, const muf_rawptr b) {
    const muf_u8 *x = (const muf_u8 *) a;
    const muf_u8 *y = (const muf_u8 *) b;
    return *x == *y;
}

muf_bool mufEqual_u16(const muf_rawptr a, const muf_rawptr b) {
    const muf_u16 *x = (const muf_u16 *) a;
    const muf_u16 *y = (const muf_u16 *) b;
    return *x == *y;
}

muf_bool mufEqual_u32(const muf_rawptr a, const muf_rawptr b) {
    const muf_u32 *x = (const muf_u32 *) a;
    const muf_u32 *y = (const muf_u32 *) b;
    return *x == *y;
}

muf_bool mufEqual_u64(const muf_rawptr a, const muf_rawptr b) {
    const muf_u64 *x = (const muf_u64 *) a;
    const muf_u64 *y = (const muf_u64 *) b;
    return *x == *y;
}

muf_bool mufEqual_str(const muf_rawptr a, const muf_rawptr b) {
    const muf_char *x = (const muf_char *) a;
    const muf_char *y = (const muf_char *) b;
    return strcmp(x, y) == 0;
}