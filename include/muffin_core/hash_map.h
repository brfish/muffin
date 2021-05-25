#ifndef _MUFFIN_CORE_HASH_MAP_H_
#define _MUFFIN_CORE_HASH_MAP_H_

#include "muffin_core/internal/hash_table.h"

typedef MufHashTable MufHashMap;
typedef MufHashTableNode MufHashMapNode;

typedef struct MufHashMapConfig_s {
    muf_usize keySize;
    muf_usize valueSize;
    MufEqualityComparator equal;
    MufHash hash;
} MufHashMapConfig;

MUF_API MufHashMap *_mufCreateHashMap(muf_usize keySize, muf_usize valueSize,
    MufEqualityComparator equal, MufHash hash);

#define mufCreateHashMap(KeyType, ValueType, Equal, Hash) _mufCreateHashMap(sizeof(KeyType), sizeof(ValueType), Equal, Hash) 

MUF_API void mufInitHashMap(MufHashMap *map, const MufHashMapConfig *config);

MUF_API MufHashMap *mufCloneHashMap(MufHashMap *map);

MUF_API void mufDestoryHashMap(MufHashMap *map);

MUF_API muf_usize mufHashMapSize(const MufHashMap *map);

MUF_API muf_usize mufHashMapBucketCount(const MufHashMap *map);

MUF_API muf_bool mufHashMapIsEmpty(const MufHashMap *map);

MUF_API muf_f32 mufHashMapGetLoadFactor(const MufHashMap *map);

MUF_API muf_bool mufHashMapGet(MufHashMap *map, muf_rawptr key, muf_rawptr valueOut);

MUF_API muf_bool mufHashMapGetRef(MufHashMap *map, muf_rawptr key, muf_rawptr *refOut);

MUF_API muf_bool mufHashMapContains(const MufHashMap *map, const muf_rawptr key);

MUF_API void mufHashMapRehash(MufHashMap *map, muf_usize newBucketCount);

MUF_API void mufHashMapInsert(MufHashMap *map, muf_rawptr key, muf_rawptr value);

MUF_API void mufHashMapInsertOrAssign(MufHashMap *map, muf_rawptr key, muf_rawptr value);

MUF_API void mufHashMapSet(MufHashMap *map, muf_rawptr key, muf_rawptr newValue);

MUF_API void mufHashMapRemove(MufHashMap *map, muf_rawptr key);

MUF_API void mufHashMapClear(MufHashMap *map);

#if defined(__GNUC__)
#   define mufHashMapGet_L(Map, Key, Out) \
        ({ __typeof__(Key) k = Key; mufHashMapGet(Map, &k, Out); })
#   define mufHashMapGetRef_L(Map, Key, Out) \
        ({ __typeof__(Key) k = Key; mufHashMapGetRef(Map, &k, Out); })
#   define mufHashMapContains_L(Map, Key) \
        ({ __typeof__(Key) k = Key; mufHashMapContains(Map, &k); })
#   define mufHashMapInsert_L(Map, Key, Value) \
        do { __typeof__(Key) k = Key; __typeof__(Value) v = Value; mufHashMapInsert(Map, &k, &v); } while (0)
#   define mufHashMapInsertOrAssign_L(Map, Key, Value) \
        do { __typeof__(Key) k = Key; __typeof__(Value) v = Value; mufHashMapInsertOrAssign(Map, &k, &v); } while (0)
#   define mufHashMapSet_L(Map, Key, NewValue) \
        do { __typeof__(Key) k = Key; __typeof__(Value) v = Value; mufHashMapSet(Map, &k, &v); } while (0)
#   define mufHashMapRemove_L(Map, Key) \
        do { __typeof__(Key) k = Key; mufHashMapRemove(Map, &k); } while (0)
#endif

#endif