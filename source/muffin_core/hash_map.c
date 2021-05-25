#include "muffin_core/hash_map.h"

#include "muffin_core/memory.h"

MUF_INTERNAL muf_rawptr _mufHashMapKeyExtract(MufHashMapNode *node) {
    return MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode), 1);
}

MufHashMap *_mufCreateHashMap(muf_usize keySize, muf_usize valueSize,
    MufEqualityComparator equal, MufHash hash) {
    mufCreateHashTable(keySize, valueSize, _mufHashMapKeyExtract, equal, hash);
}

void mufDestoryHashMap(MufHashMap *map) {
    mufDestroyHashTable(map);
}

muf_usize mufHashMapSize(const MufHashMap *map) {
    return map->size;
}

muf_usize mufHashMapBucketCount(const MufHashMap *map) {
    return map->bucketCount;
}

muf_bool mufHashMapIsEmpty(const MufHashMap *map) {
    return map->size == 0;
}

muf_f32 mufHashMapGetLoadFactor(const MufHashMap *map) {
    return mufHashTableGetLoadFactor(map);
}

muf_bool mufHashMapGet(MufHashMap *map, muf_rawptr key, muf_rawptr valueOut) {
    MufHashMapNode *node = mufHashTableFind(map, key, NULL);
    if (node == NULL)
        return MUF_FALSE;
    muf_rawptr value = MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode) + map->keySize, 1);
    memcpy(valueOut, value, map->valueSize);
    return MUF_TRUE;
}

muf_bool mufHashMapGetRef(MufHashMap *map, muf_rawptr key, muf_rawptr *refOut) {
    MufHashMapNode *node = mufHashTableFind(map, key, NULL);
    if (node == NULL) {
        *refOut = NULL;
        return MUF_FALSE;
    }
    *refOut = MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode) + map->keySize, 1);
    return MUF_TRUE;
}

muf_bool mufHashMapContains(const MufHashMap *map, const muf_rawptr key) {
    return mufHashTableFind((MufHashMap *)map, key, NULL) != NULL;
}

void mufHashMapRehash(MufHashMap *map, muf_usize newBucketCount) {
    mufHashTableRehash(map, newBucketCount);
}

void mufHashMapInsert(MufHashMap *map, muf_rawptr key, muf_rawptr value) {
    MufHashMapNode *node = (MufHashMapNode *) mufAllocate(muf_byte, sizeof(MufHashMapNode) + map->keySize + map->valueSize);
    node->next = NULL;
    node->hashCode = map->hash(key);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode), 1), key, map->keySize);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode) + map->keySize, 1), value, map->valueSize);
    mufHashTableInsert(map, node);    
}

void mufHashMapInsertOrAssign(MufHashMap *map, muf_rawptr key, muf_rawptr value) {
    MufHashMapNode *node = (MufHashMapNode *) mufAllocate(muf_byte, sizeof(MufHashMapNode) + map->keySize + map->valueSize);
    node->next = NULL;
    node->hashCode = map->hash(key);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode), 1), key, map->keySize);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashMapNode) + map->keySize, 1), value, map->valueSize);
    mufHashTableInsertOrAssign(map, node);   
}

void mufHashMapSet(MufHashMap *map, muf_rawptr key, muf_rawptr newValue) {
    mufHashMapInsertOrAssign(map, key, newValue);
}

void mufHashMapRemove(MufHashMap *map, muf_rawptr key) {
    mufHashTableRemove(map, key);
}

void mufHashMapClear(MufHashMap *map) {
    mufHashTableClear(map);
}