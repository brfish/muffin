#ifndef _MUFFIN_CORE_INTERNAL_HASH_TABLE_H_
#define _MUFFIN_CORE_INTERNAL_HASH_TABLE_H_

#include "muffin_core/common.h"
#include "muffin_core/hash.h"

typedef struct MufHashTableNode_s MufHashTableNode;

struct MufHashTableNode_s {
    MufHashTableNode    *next;
    muf_index           hashCode;
};

#define mufHashTableNodeGetData(_node) MUF_RAWPTR_AT(_node, sizeof(MufHashTableNode), 1)

MufHashTableNode *mufCreateHashTableNode(MufHashTableNode *next, muf_index hashCode, muf_usize dataSize, muf_rawptr data);

typedef muf_rawptr(*MufKeyExtractor)(MufHashTableNode *);

typedef struct MufHashTable_s {
    muf_usize               keySize;
    muf_usize               valueSize;
    muf_usize               storageSize;
    MufKeyExtractor         keyExtract;
    MufEqualityComparator   equal;
    MufHash                 hash;

    MufHashTableNode        **buckets;
    muf_usize               bucketCount;
    muf_usize               size;
} MufHashTable;

MufHashTable *mufCreateHashTable(muf_usize keySize, muf_usize valueSize, MufKeyExtractor keyExtract,
    MufEqualityComparator equal, MufHash hash);

void mufInitHashTable(MufHashTable *table);

MufHashTable *mufCloneHashTable(const MufHashTable *table);

void mufDestroyHashTable(MufHashTable *table);

muf_f32 mufHashTableGetLoadFactor(const MufHashTable *table);

MufHashTableNode *mufHashTableFind(const MufHashTable *table, muf_crawptr key, MufHashTableNode **prevOut);

static MUF_INLINE muf_rawptr mufHashTableExtractNodeKey(const MufHashTable *table, MufHashTableNode *node);

static MUF_INLINE muf_rawptr mufHashTableExtractNodeValue(const MufHashTable *table, MufHashTableNode *node);

void mufHashTableInsert(MufHashTable *table, MufHashTableNode *node);

muf_bool mufHashTableRemove(MufHashTable *table, muf_crawptr key);

void mufHashTableClear(MufHashTable *table);

void mufHashTableRehash(MufHashTable *table, muf_usize newBucketCount);

void mufHashTableForEach(MufHashTable *table, void (*unaryFunc)(muf_rawptr item));

static MUF_INLINE muf_rawptr mufHashTableExtractNodeKey(const MufHashTable *table, MufHashTableNode *node) {
    return MUF_RAWPTR_TYPE_AT(node, MufHashTableNode, 1);
}

static MUF_INLINE muf_rawptr mufHashTableExtractNodeValue(const MufHashTable *table, MufHashTableNode *node) {
    return MUF_RAWPTR_AT(node, sizeof(MufHashTableNode) + table->keySize, 1);
}

#endif