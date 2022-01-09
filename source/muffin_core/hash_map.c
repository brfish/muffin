#include "muffin_core/hash_map.h"

#include "internal/hash_table.h"
#include "muffin_core/memory.h"

#define _TABLE(_ptr) ((MufHashTable *)(_ptr))

static muf_rawptr _mufHashMapKeyExtract(MufHashTableNode *node) {
    return MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashTableNode), 1);
}

MufHashMap *_mufCreateHashMap(muf_usize keySize, muf_usize valueSize,
    MufEqualityComparator equal, MufHash hash) {
    return (MufHashMap *) mufCreateHashTable(keySize, valueSize, _mufHashMapKeyExtract, equal, hash);
}

void mufDestoryHashMap(MufHashMap *map) {
    mufDestroyHashTable(_TABLE(map));
}

muf_usize mufHashGetMapSize(const MufHashMap *map) {
    return _TABLE(map)->size;
}

muf_usize mufHashMapGetBucketCount(const MufHashMap *map) {
    return _TABLE(map)->bucketCount;
}

muf_bool mufHashMapIsEmpty(const MufHashMap *map) {
    return _TABLE(map)->size == 0;
}

muf_f32 mufHashMapGetLoadFactor(const MufHashMap *map) {
    return mufHashTableGetLoadFactor(_TABLE(map));
}

muf_bool mufHashMapGet(const MufHashMap *map, muf_crawptr key, muf_rawptr valueOut) {
    MufHashTable *table = _TABLE(map);
    MufHashTableNode *node = mufHashTableFind(table, key, NULL);
    if (node == NULL)
        return MUF_FALSE;
    muf_rawptr value = MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashTableNode) + table->keySize, 1);
    memcpy(valueOut, value, table->valueSize);
    return MUF_TRUE;
}

muf_rawptr mufHashMapGetRef(MufHashMap *map, muf_crawptr key) {
    MufHashTable *table = _TABLE(map);
    MufHashTableNode *node = mufHashTableFind(table, key, NULL);
    return node == NULL ? NULL : mufHashTableNodeGetData(node);
    if (node == NULL) {
        return NULL;
    }
    return MUF_RAWPTR_AT(node, sizeof(MufHashTableNode) + table->keySize, 1);
}

muf_crawptr mufHashMapGetCRef(const MufHashMap *map, muf_crawptr key) {
    MufHashTable *table = _TABLE(map);
    MufHashTableNode *node = mufHashTableFind(table, key, NULL);
    if (node == NULL) {
        return NULL;
    }
    return MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashTableNode) + table->keySize, 1);
}

muf_bool mufHashMapContains(const MufHashMap *map, muf_crawptr key) {
    return mufHashTableFind(_TABLE(map), key, NULL) != NULL;
}

void mufHashMapRehash(MufHashMap *map, muf_usize newBucketCount) {
    mufHashTableRehash(_TABLE(map), newBucketCount);
}

static MUF_INLINE MufHashTableNode *_mufHashMapCreateNode(const MufHashMap *map, muf_crawptr key, muf_crawptr value) {
    MufHashTable *table = _TABLE(map);
    MufHashTableNode *node = mufCreateHashTableNode(NULL, table->hash(key), table->storageSize, NULL);
    muf_rawptr keyDst = MUF_RAWPTR_AT(node, sizeof(MufHashTableNode), 1);
    muf_rawptr valueDst = MUF_RAWPTR_AT(node, sizeof(MufHashTableNode) + table->keySize, 1);
    mufMemCopyBytes(keyDst, key, table->keySize);
    mufMemCopyBytes(valueDst, value, table->valueSize);
    return node;
}

muf_bool mufHashMapInsert(MufHashMap *map, muf_crawptr key, muf_crawptr value) {
    if (mufHashMapContains(map, key)) {
        return MUF_FALSE;
    }
    MufHashTableNode *node = _mufHashMapCreateNode(map, key, value);
    mufHashTableInsert(_TABLE(map), node);    
    return MUF_TRUE;
}

void mufHashMapInsertOrAssign(MufHashMap *map, muf_crawptr key, muf_crawptr value) {
    MufHashTable *table = _TABLE(map);
    MufHashTableNode *node = mufHashTableFind(table, key, NULL);
    if (node) {
        muf_rawptr valueDst = MUF_RAWPTR_AT(node, sizeof(MufHashTableNode) + table->keySize, 1);
        mufMemCopyBytes(valueDst, value, table->valueSize);
    } else {
        MufHashTableNode *node = _mufHashMapCreateNode(map, key, value);
        mufHashTableInsert(table, node);
    }
}

void mufHashMapSet(MufHashMap *map, muf_crawptr key, muf_crawptr newValue) {
    mufHashMapInsertOrAssign(map, key, newValue);
}

muf_bool mufHashMapRemove(MufHashMap *map, muf_crawptr key) {
    return mufHashTableRemove(_TABLE(map), key);
}

void mufHashMapClear(MufHashMap *map) {
    mufHashTableClear(_TABLE(map));
}

#undef _TABLE