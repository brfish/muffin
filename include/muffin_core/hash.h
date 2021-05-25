#ifndef _MUFFIN_CORE_HASH_H_
#define _MUFFIN_CORE_HASH_H_

#include "muffin_core/common.h"

typedef muf_index(*MufHash)(const muf_rawptr);

MUF_API muf_index mufHash_i8(const muf_rawptr value);

MUF_API muf_index mufHash_i16(const muf_rawptr value);

MUF_API muf_index mufHash_i32(const muf_rawptr value);

MUF_API muf_index mufHash_i64(const muf_rawptr value);

MUF_API muf_index mufHash_u8(const muf_rawptr value);

MUF_API muf_index mufHash_u16(const muf_rawptr value);

MUF_API muf_index mufHash_u32(const muf_rawptr value);

MUF_API muf_index mufHash_u64(const muf_rawptr value);

MUF_API muf_index mufHash_f32(const muf_rawptr value);

MUF_API muf_index mufHash_f64(const muf_rawptr value);

MUF_API muf_index mufHash_str(const muf_rawptr value);

MUF_API muf_index mufHash_bytes(const muf_rawptr data, muf_usize length);

#endif