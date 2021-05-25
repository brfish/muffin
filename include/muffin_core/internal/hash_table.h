#ifndef _MUFFIN_CORE_INTERNAL_HASH_TABLE_H_
#define _MUFFIN_CORE_INTERNAL_HASH_TABLE_H_

#include "muffin_core/common.h"
#include "muffin_core/hash.h"

typedef struct MufHashTableNode_s MufHashTableNode;

struct MufHashTableNode_s {
    MufHashTableNode *next;
    muf_index hashCode;
};

typedef muf_rawptr(*MufKeyExtractor)(MufHashTableNode *);

typedef struct MufHashTable_s {
    muf_usize keySize;
    muf_usize valueSize;
    muf_usize storageSize;
    MufKeyExtractor keyExtract;
    MufEqualityComparator equal;
    MufHash hash;

    MufHashTableNode **buckets;
    muf_usize bucketCount;
    muf_usize size;
} MufHashTable;

MufHashTable *mufCreateHashTable(muf_usize keySize, muf_usize valueSize, MufKeyExtractor keyExtract,
    MufEqualityComparator equal, MufHash hash);

void mufInitHashTable(MufHashTable *table);

MufHashTable *mufCloneHashTable(const MufHashTable *table);

void mufDestroyHashTable(MufHashTable *table);

muf_f32 mufHashTableGetLoadFactor(const MufHashTable *table);

MufHashTableNode *mufHashTableFind(MufHashTable *table, const muf_rawptr key, MufHashTableNode **prevOut);

void mufHashTableInsert(MufHashTable *table, MufHashTableNode *node);

void mufHashTableInsertOrAssign(MufHashTable *table, MufHashTableNode *node);

void mufHashTableRemove(MufHashTable *table, const muf_rawptr key);

void mufHashTableClear(MufHashTable *table);

void mufHashTableRehash(MufHashTable *table, muf_usize newBucketCount);


#endif