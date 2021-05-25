#include "muffin_core/hash.h"

#include <string.h>

MUF_INTERNAL muf_u32 _mufBytesHash32(const muf_rawptr data, muf_usize length) {
    const muf_u32 m = 0x5BD1E995;
    const muf_u32 r = 24;
    const muf_u32 seed = 0xBF114514;

    muf_u32 h = seed ^ length;
    const muf_byte *bytes = (const muf_byte *) data;

    while (length >= 4) {
        muf_u32 k = bytes[0] |
            (bytes[1] << 8) |
            (bytes[2] << 16) |
            (bytes[3] << 24);
        k *= m;
        k ^= k >> r;
        k *= m;
        k *= m;
        h ^= k;

        bytes += 4;
        length -= 4;
    }

    switch (length) {
    case 3: 
        h ^= (bytes[2] & 0xFF) << 16;
    case 2:
        h ^= (bytes[1] & 0xFF) << 8;
    case 1:
        h ^= (bytes[0] & 0xFF);
        h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

MUF_INTERNAL muf_u64 _mufBytesHash64(const muf_rawptr data, muf_usize length) {
    const muf_u64 m = 0xC6A4A7935BD1E995ULL;
    const muf_u32 r = 47;
    const muf_u64 seed = 0xBF114514ULL;

    muf_u64 h = seed ^ (length * m);
    const muf_u64 *first = (const muf_u64 *) data;
    const muf_u64 *last = first + (length / 8);
    
    while (first != last) {
        muf_u64 k = *first++;
        
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
    }

    const muf_byte *bytes = (const muf_byte *) first;

    switch (length & 7) {
    case 7:
        h ^= ((muf_u64) bytes[6]) << 48;
    case 6:
        h ^= ((muf_u64) bytes[5]) << 40;
    case 5:
        h ^= ((muf_u64) bytes[4]) << 32;
    case 4:
        h ^= ((muf_u64) bytes[3]) << 24;
    case 3:
        h ^= ((muf_u64) bytes[2]) << 16;
    case 2:
        h ^= ((muf_u64) bytes[1]) << 8;
    case 1:
        h ^= ((muf_u64) bytes[0]);
        h *= m;
    }
    
    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

#if MUF_SIZEOF_USIZE == 4
#   define _mufBytesHash(data, length) _mufBytesHash32(data, length)
#elif MUF_SIZEOF_USIZE == 8
#   define _mufBytesHash(data, length) _mufBytesHash64(data, length)
#endif

MUF_INTERNAL muf_index _mufHashInteger(muf_index value) {
    return value;
}

muf_index mufHash_i8(const muf_rawptr value) {
    const muf_i8 *v = (const muf_i8 *) value;
    return _mufBytesHash32(value, sizeof(muf_i8));
}

muf_index mufHash_i16(const muf_rawptr value) {
    const muf_i16 *v = (const muf_i16 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_i32(const muf_rawptr value) {
    const muf_i32 *v = (const muf_i32 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_i64(const muf_rawptr value) {
    const muf_i64 *v = (const muf_i64 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_u8(const muf_rawptr value) {
    const muf_u8 *v = (const muf_u8 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_u16(const muf_rawptr value) {
    const muf_u16 *v = (const muf_u16 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_u32(const muf_rawptr value) {
    const muf_u32 *v = (const muf_u32 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_u64(const muf_rawptr value) {
    const muf_u64 *v = (const muf_u64 *) value;
    return _mufHashInteger((muf_index) (*v));
}

muf_index mufHash_f32(const muf_rawptr value) {
    return _mufBytesHash(value, sizeof(muf_f32));
}

muf_index mufHash_f64(const muf_rawptr value) {
    return _mufBytesHash(value, sizeof(muf_f64));
}

muf_index mufHash_str(const muf_rawptr value) {
    muf_usize length = strlen((const muf_char *) value);
    return _mufBytesHash(value, length);
}

muf_index mufHash_bytes(const muf_rawptr data, muf_usize length) {
    return _mufBytesHash(data, length);
}