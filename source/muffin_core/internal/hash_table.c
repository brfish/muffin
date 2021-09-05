#include "hash_table.h"

#include <math.h>

#include "muffin_core/math.h"
#include "muffin_core/memory.h"

#define MUF_HASHTABLE_DEFAULT_INIT_BUCKET_COUNT 8
#define MUF_HASHTABLE_DEFAULT_MAX_LOAD_FACTOR 1.0F

static MUF_INLINE muf_index _mufHashTableCalcBucketIndex(const MufHashTable *table, muf_index hashCode) {
    return hashCode & (table->bucketCount - 1);
}

MufHashTableNode *mufCreateHashTableNode(MufHashTableNode *next, muf_index hashCode, muf_usize storageSize, muf_rawptr storage) {
    MufHashTableNode *node = (MufHashTableNode *) mufAllocBytes(sizeof(MufHashTableNode) + storageSize);
    node->next = next;
    node->hashCode = hashCode;
    if (storage) {
        muf_rawptr dst = mufHashTableNodeGetData(node);
        mufMemCopyBytes(dst, storage, storageSize);
    }
    return node;
}

MufHashTable *mufCreateHashTable(muf_usize keySize, muf_usize valueSize, MufKeyExtractor keyExtract,
    MufEqualityComparator equal, MufHash hash) {
    MufHashTable *table = mufAlloc(MufHashTable, 1);
    table->keySize = keySize;
    table->valueSize = valueSize;
    table->storageSize = keySize + valueSize;
    table->keyExtract = keyExtract;
    table->equal = equal;
    table->hash = hash;
    table->bucketCount = MUF_HASHTABLE_DEFAULT_INIT_BUCKET_COUNT;
    table->buckets = mufAllocZero(MufHashTableNode *, table->bucketCount);
    table->size = 0;
    return table;
}

void mufDestroyHashTable(MufHashTable *table) {
    if (table == NULL) {
        return;
    }

    mufHashTableClear(table);
    mufFree(table->buckets);
    mufFree(table);
}

muf_f32 mufHashTableGetLoadFactor(const MufHashTable *table) {
    return ((muf_f32) table->size) / table->bucketCount;
}

MufHashTableNode *mufHashTableFind(const MufHashTable *table, muf_crawptr key, MufHashTableNode **prevOut) {
    muf_index hashCode = table->hash(key);
    muf_index bucketIndex = _mufHashTableCalcBucketIndex(table, hashCode);

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
    if (((muf_f32) (table->size + 1)) / table->bucketCount >= MUF_HASHTABLE_DEFAULT_MAX_LOAD_FACTOR)
        mufHashTableRehash(table, table->size + 1);
    
    muf_index bucketIndex = node->hashCode & (table->bucketCount - 1);
    node->next = table->buckets[bucketIndex];
    table->buckets[bucketIndex] = node;
    ++table->size;
}

muf_bool mufHashTableRemove(MufHashTable *table, muf_crawptr key) {
    MufHashTableNode *prev = NULL;
    MufHashTableNode *node = mufHashTableFind(table, key, &prev);

    if (node == NULL)
        return MUF_FALSE;
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        muf_index bucketIndex = _mufHashTableCalcBucketIndex(table, node->hashCode);
        table->buckets[bucketIndex] = node->next;
    }
    --table->size;
    mufFree(node);
    return MUF_TRUE;
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

    MufHashTableNode **newBuckets = mufAllocZero(MufHashTableNode *, newBucketCount);

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

void mufHashTableForEach(MufHashTable *table, void (*unaryFunc)(muf_rawptr item)) {
    for (muf_index i = 0; i < table->bucketCount; ++i) {
        MufHashTableNode *node = table->buckets[i];
        while (node) {
            MufHashTableNode *next = node->next;
            unaryFunc(mufHashTableExtractNodeValue(table, node));
            node = next;
        }
    }
}