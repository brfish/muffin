#ifndef _MUFFIN_CORE_HASH_MAP_H_
#define _MUFFIN_CORE_HASH_MAP_H_

#include "muffin_core/common.h"
#include "muffin_core/hash.h"

typedef struct MufHashMap_s MufHashMap;

typedef struct MufHashMapConfig_s {
    muf_usize                   keySize;
    muf_usize                   valueSize;
    MufEqualityComparator       equal;
    MufHash                     hash;
} MufHashMapConfig;

MUF_API MufHashMap *_mufCreateHashMap(muf_usize keySize, muf_usize valueSize,
    MufEqualityComparator equal, MufHash hash);

#define mufCreateHashMap(_keyType, _valueType, _equal, _hash) _mufCreateHashMap(sizeof(_keyType), sizeof(_valueType), _equal, _hash) 

MUF_API void mufInitHashMap(MufHashMap *map, const MufHashMapConfig *config);

MUF_API MufHashMap *mufCloneHashMap(MufHashMap *map);

MUF_API void mufDestoryHashMap(MufHashMap *map);

MUF_API muf_usize mufHashMapGetSize(const MufHashMap *map);

MUF_API muf_usize mufHashMapGetBucketCount(const MufHashMap *map);

MUF_API muf_bool mufHashMapIsEmpty(const MufHashMap *map);

MUF_API muf_f32 mufHashMapGetLoadFactor(const MufHashMap *map);

MUF_API muf_bool mufHashMapGet(const MufHashMap *map, muf_crawptr key, muf_rawptr valueOut);

MUF_API muf_rawptr mufHashMapGetRef(MufHashMap *map, muf_crawptr key);

MUF_API muf_crawptr mufHashMapGetCRef(const MufHashMap *map, muf_crawptr key);

MUF_API muf_bool mufHashMapContains(const MufHashMap *map, muf_crawptr key);

MUF_API void mufHashMapRehash(MufHashMap *map, muf_usize newBucketCount);

MUF_API muf_bool mufHashMapInsert(MufHashMap *map, muf_crawptr key, muf_crawptr value);

MUF_API void mufHashMapInsertOrAssign(MufHashMap *map, muf_crawptr key, muf_crawptr value);

MUF_API void mufHashMapSet(MufHashMap *map, muf_crawptr key, muf_crawptr newValue);

MUF_API muf_bool mufHashMapRemove(MufHashMap *map, muf_crawptr key);

MUF_API void mufHashMapClear(MufHashMap *map);

MUF_API void mufHashMapForEach(MufHashMap *map, MufBinary2Func(func, void, muf_crawptr, muf_rawptr));

#if defined(__GNUC__)
#   define mufHashMapGet_L(_map, _key, _out) \
        ({ __typeof__(_key) k = _key; mufHashMapGet(_map, &k, _out); })
#   define mufHashMapGetRef_L(_map, _key, _out) \
        ({ __typeof__(_key) k = _key; mufHashMapGetRef(_map, &k, _out); })
#   define mufHashMapGetCRef_L(_map, _key, _out) \
        ({ __typeof__(_key) k = _key; mufHashMapGetCRef(_map, &k, _out); })
#   define mufHashMapContains_L(_map, _key) \
        ({ __typeof__(_key) k = _key; mufHashMapContains(_map, &k); })
#   define mufHashMapInsert_L(_map, _key, _value) \
        ({ __typeof__(_key) k = _key; __typeof__(_value) v = _value; mufHashMapInsert(_map, &k, &v); })
#   define mufHashMapInsertOrAssign_L(_map, _key, _value) \
        do { __typeof__(_key) k = _key; __typeof__(_value) v = _value; mufHashMapInsertOrAssign(_map, &k, &v); } while (0)
#   define mufHashMapSet_L(_map, _key, NewValue) \
        do { __typeof__(_key) k = _key; __typeof__(_value) v = _value; mufHashMapSet(_map, &k, &v); } while (0)
#   define mufHashMapRemove_L(_map, _key) \
        ({ __typeof__(_key) k = _key; mufHashMapRemove(_map, &k); })
#endif

#define mufCreateHashMap_i8 (_valueType) mufCreateHashMap(muf_i8,  _valueType, mufEqual_i8,  mufHash_i8)
#define mufCreateHashMap_i16(_valueType) mufCreateHashMap(muf_i16, _valueType, mufEqual_i16, mufHash_i16)
#define mufCreateHashMap_i32(_valueType) mufCreateHashMap(muf_i32, _valueType, mufEqual_i32, mufHash_i32)
#define mufCreateHashMap_i64(_valueType) mufCreateHashMap(muf_i64, _valueType, mufEqual_i64, mufHash_i64)
#define mufCreateHashMap_u8 (_valueType) mufCreateHashMap(muf_u8,  _valueType, mufEqual_u8,  mufHash_u8)
#define mufCreateHashMap_u16(_valueType) mufCreateHashMap(muf_u16, _valueType, mufEqual_u16, mufHash_u16)
#define mufCreateHashMap_u32(_valueType) mufCreateHashMap(muf_u32, _valueType, mufEqual_u32, mufHash_u32)
#define mufCreateHashMap_u64(_valueType) mufCreateHashMap(muf_u64, _valueType, mufEqual_u64, mufHash_u64)

#endif