#include "muffin_core/internal/hash_table.h"

#include <math.h>

#include "muffin_core/math.h"
#include "muffin_core/memory.h"

#define MUF_HASHTABLE_DEFAULT_INIT_BUCKET_COUNT 8
#define MUF_HASHTABLE_DEFAULT_MAX_LOAD_FACTOR 1.0F

MufHashTable *mufCreateHashTable(muf_usize keySize, muf_usize valueSize, MufKeyExtractor keyExtract,
    MufEqualityComparator equal, MufHash hash) {
    MufHashTable *table = mufAllocate(MufHashTable, 1);
    table->keySize = keySize;
    table->valueSize = valueSize;
    table->storageSize = keySize + valueSize;
    table->keyExtract = keyExtract;
    table->equal = equal;
    table->hash = hash;
    table->bucketCount = MUF_HASHTABLE_DEFAULT_INIT_BUCKET_COUNT;
    table->buckets = mufAllocateZero(MufHashTableNode *, table->bucketCount);
    table->size = 0;
}

void mufDestroyHashTable(MufHashTable *table) {
#if defined(MUF_DEBUG)
    if (table == NULL)
        return;
#endif
    mufHashTableClear(table);
    mufFree(table->buckets);
    mufFree(table);
}

muf_f32 mufHashTableGetLoadFactor(const MufHashTable *table) {
    return ((muf_f32) table->size) / table->bucketCount;
}

MufHashTableNode *mufHashTableFind(MufHashTable *table, const muf_rawptr key, MufHashTableNode **prevOut) {
    muf_index hashCode = table->hash(key);
    muf_index bucketIndex = hashCode & (table->bucketCount - 1);

    MufHashTableNode *prv = NULL;
    MufHashTableNode *cur = table->buckets[bucketIndex];
    while (cur) {
        muf_rawptr nodeKey = table->keyExtract(cur);
        if (table->equal(nodeKey, key)) {
            if (prevOut != NULL)
                *prevOut = prv;
            return cur;
        }
        prv = cur;
        cur = cur->next;
    }
    return NULL;
}

void mufHashTableInsert(MufHashTable *table, MufHashTableNode *node) {
#if defined(MUF_DEBUG)
    MUF_FASSERT(mufHashTableFind(table, table->keyExtract(node), NULL) == NULL, 
        "The element already exists in the table, it's better to use insertOrAssign method");
#endif

    if (((muf_f32) (table->size + 1)) / table->bucketCount >= MUF_HASHTABLE_DEFAULT_MAX_LOAD_FACTOR)
        mufHashTableRehash(table, table->size + 1);
    
    muf_index bucketIndex = node->hashCode & (table->bucketCount - 1);
    node->next = table->buckets[bucketIndex];
    table->buckets[bucketIndex] = node;
    ++table->size;
}

void mufHashTableInsertOrAssign(MufHashTable *table, MufHashTableNode *node) {
    MufHashTableNode *oldNode = mufHashTableFind(table, table->keyExtract(node), NULL);
    if (oldNode) {
        memcpy(oldNode, node, sizeof(MufHashTableNode) + table->valueSize);
    } else {
        mufHashTableInsert(table, node);
    }
}

void mufHashTableRemove(MufHashTable *table, const muf_rawptr key) {
    MufHashTableNode *prev = NULL;
    MufHashTableNode *node = mufHashTableFind(table, key, &prev);

    if (node == NULL)
        return;
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        muf_index bucketIndex = node->hashCode & (table->bucketCount - 1);
        table->buckets[bucketIndex] = node->next;
    }
    --table->size;
    mufFree(node);
}

void mufHashTableClear(MufHashTable *table) {
    for (muf_index i = 0; i < table->bucketCount; ++i) {
        MufHashTableNode *node = table->buckets[i];
        while (node) {
            MufHashTableNode *next = node->next;
            mufFree(node);
            node = next;
        }
    }
    table->size = 0;
}

void mufHashTableRehash(MufHashTable *table, muf_usize newBucketCount) {
    if (!(newBucketCount & (newBucketCount - 1)))
        newBucketCount = mufCeilPower2(newBucketCount);

    MufHashTableNode **newBuckets = mufAllocateZero(MufHashTableNode *, newBucketCount);

    for (muf_index i = 0; i < table->bucketCount; ++i) {
        MufHashTableNode *node = table->buckets[i];
        while (node) {
            muf_index newBucketIndex = node->hashCode & (newBucketCount - 1);
            MufHashTableNode *nextNode = node->next;

            node->next = newBuckets[newBucketIndex];
            newBuckets[newBucketIndex] = node;
            node = nextNode;
        }
    }
    mufFree(table->buckets);
    table->buckets = newBuckets;
    table->bucketCount = newBucketCount;
}