#ifndef _MUFFIN_CORE_HASH_H_
#define _MUFFIN_CORE_HASH_H_

#include "muffin_core/common.h"

typedef muf_index(*MufHash)(muf_crawptr);

MUF_API muf_index mufHash_i8 (muf_crawptr value);
MUF_API muf_index mufHash_i16(muf_crawptr value);
MUF_API muf_index mufHash_i32(muf_crawptr value);
MUF_API muf_index mufHash_i64(muf_crawptr value);
MUF_API muf_index mufHash_u8 (muf_crawptr value);
MUF_API muf_index mufHash_u16(muf_crawptr value);
MUF_API muf_index mufHash_u32(muf_crawptr value);
MUF_API muf_index mufHash_u64(muf_crawptr value);
MUF_API muf_index mufHash_f32(muf_crawptr value);
MUF_API muf_index mufHash_f64(muf_crawptr value);
MUF_API muf_index mufHash_str(muf_crawptr value);
MUF_API muf_index mufHash_ptr(muf_crawptr value);
MUF_API muf_index mufHashBytes(muf_crawptr data, muf_usize length);

#endif