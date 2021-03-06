#ifndef _MUFFIN_CORE_HASH_SET_H_
#define _MUFFIN_CORE_HASH_SET_H_

#include "muffin_core/common.h"
#include "muffin_core/hash.h"

typedef struct MufHashSet_s MufHashSet;

typedef struct MufHashSetConfig_s {
    muf_usize                   elementSize;
    MufEqualityComparator       equal;
    MufHash                     hash;
    muf_usize                   initBucketCount;
} MufHashSetConfig;

MUF_API MufHashSet *_mufCreateHashSet(muf_usize elementSize, MufEqualityComparator equal, MufHash hash);

#define mufCreateHashSet(_type, _equal, _hash) _mufCreateHashSet(sizeof(_type), _equal, _hash)

MUF_API void mufInitHashSet(MufHashSet *set, const MufHashSetConfig *config);

MUF_API void mufDestroyHashSet(MufHashSet *set);

MUF_API MufHashSet *mufCloneHashSet(const MufHashSet *set);

MUF_API muf_usize mufHashSetGetSize(const MufHashSet *set);

MUF_API muf_usize mufHashSetGetBucketCount(const MufHashSet *set);

MUF_API muf_bool mufHashSetIsEmpty(const MufHashSet *set);

MUF_API muf_f32 mufHashSetGetLoadFactor(const MufHashSet *set);

MUF_API muf_rawptr mufHashSetFind(MufHashSet *set, muf_crawptr item);

MUF_API muf_crawptr mufHashSetCFind(const MufHashSet *set, muf_crawptr item);

MUF_API muf_bool mufHashSetContains(const MufHashSet *set, muf_crawptr item);

MUF_API muf_bool mufHashSetInsert(MufHashSet *set, muf_crawptr item);

MUF_API void mufHashSetInsertOrAssign(MufHashSet *set, muf_crawptr item);

MUF_API muf_usize mufHashSetInsertRange(MufHashSet *set, muf_crawptr items, muf_usize count);

MUF_API muf_bool mufHashSetRemove(MufHashSet *set, muf_crawptr item);

MUF_API void mufHashSetClear(MufHashSet *set);

MUF_API void mufHashSetRehash(MufHashSet *set, muf_usize newBucketCount);

void mufHashSetForEach(MufHashSet *set, void (*unaryFunc)(muf_rawptr item));

#if defined(__GNUC__)
#   define mufHashSetFind_L(_set, _item) \
        ({ __typeof__(_item) tmp = _item; mufHashSetFind(_set, &tmp); })
#   define mufHashSetContains_L(_set, _item) \
        ({ __typeof__(_item) tmp = _item; mufHashSetContains(_set, &tmp); })
#   define mufHashSetInsert_L(_set, _item) \
        ({ __typeof__(_item) tmp = _item; mufHashSetInsert(_set, &tmp); })
#   define mufHashSetInsertOrAssign_L(_set, _item) \
        do { __typeof__(_item) tmp = _item; mufHashSetInsertOrAssign(_set, &tmp); } while (0)
#   define mufHashSetRemove_L(_set, _item) \
        ({ __typeof__(_item) tmp = _item; mufHashSetRemove(_set, &tmp); })
#endif

#define mufCreateHashSet_i8 () mufCreateHashSet(muf_i8,  mufEqual_i8,  mufHash_i8 )
#define mufCreateHashSet_i16() mufCreateHashSet(muf_i16, mufEqual_i16, mufHash_i16)
#define mufCreateHashSet_i32() mufCreateHashSet(muf_i32, mufEqual_i32, mufHash_i32)
#define mufCreateHashSet_i64() mufCreateHashSet(muf_i64, mufEqual_i64, mufHash_i64)
#define mufCreateHashSet_u8 () mufCreateHashSet(muf_u8,  mufEqual_u8,  mufHash_u8 )
#define mufCreateHashSet_u16() mufCreateHashSet(muf_u16, mufEqual_u16, mufHash_u16)
#define mufCreateHashSet_u32() mufCreateHashSet(muf_u32, mufEqual_u32, mufHash_u32)
#define mufCreateHashSet_u64() mufCreateHashSet(muf_u64, mufEqual_u64, mufHash_u64)

#endif