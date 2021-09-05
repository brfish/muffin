#include "muffin_core/hash_set.h"

#include "internal/hash_table.h"
#include "muffin_core/memory.h"

#define _CTABLE(Ptr) ((const MufHashTable *) Ptr)
#define _TABLE(Ptr) ((MufHashTable *) Ptr)

MUF_INTERNAL muf_rawptr _mufHashSetKeyExtract(MufHashTableNode *node) {
    return MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashTableNode), 1);
}

MufHashSet *_mufCreateHashSet(muf_usize elementSize, MufEqualityComparator equal, MufHash hash) {
    return (MufHashSet *) mufCreateHashTable(elementSize, 0, _mufHashSetKeyExtract, equal, hash);
}

void mufDestroyHashSet(MufHashSet *set) {
    mufDestroyHashTable(_TABLE(set));
}

MufHashSet *mufCloneHashSet(const MufHashSet *set) {
    MufHashSet *clone = _mufCreateHashSet(_CTABLE(set)->keySize, _CTABLE(set)->equal, _CTABLE(set)->hash);
    return clone;
}

muf_usize mufHashSetGetSize(const MufHashSet *set) {
    return _CTABLE(set)->size;
}

muf_usize mufHashSetGetBucketCount(const MufHashSet *set) {
    return _CTABLE(set)->bucketCount;
}

muf_bool mufHashSetIsEmpty(const MufHashSet *set) {
    return _CTABLE(set)->size == 0;
}

muf_f32 mufHashSetGetLoadFactor(const MufHashSet *set) {
    return mufHashTableGetLoadFactor(_CTABLE(set));
}

muf_rawptr mufHashSetFind(MufHashSet *set, muf_crawptr item) {
    MufHashTableNode *node = mufHashTableFind(_TABLE(set), item, NULL);
    return _mufHashSetKeyExtract(node);
}

muf_crawptr mufHashSetCFind(const MufHashSet *set, muf_crawptr item) {
    MufHashTableNode *node = mufHashTableFind(_CTABLE(set), item, NULL);
    return (muf_crawptr) _mufHashSetKeyExtract(node);
}

muf_bool mufHashSetContains(const MufHashSet *set, muf_crawptr item) {
    return mufHashSetCFind(set, item) != NULL;
}

muf_bool mufHashSetInsert(MufHashSet *set, muf_crawptr item) {
    if (mufHashSetContains(set, item)) {
        return MUF_FALSE;
    }
    MufHashTableNode *node = (MufHashTableNode *) mufAlloc(muf_byte, sizeof(MufHashTableNode) + _TABLE(set)->keySize);
    node->next = NULL;
    node->hashCode = _TABLE(set)->hash(item);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashTableNode), 1), item, _TABLE(set)->keySize);
    mufHashTableInsert(_TABLE(set), node);
    return MUF_TRUE;
}

void mufHashSetInsertOrAssign(MufHashSet *set, muf_crawptr item) {
    MufHashTableNode *node = (MufHashTableNode *) mufAlloc(muf_byte, sizeof(MufHashTableNode) + _TABLE(set)->keySize);
    node->next = NULL;
    node->hashCode = _TABLE(set)->hash(item);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashTableNode), 1), item, _TABLE(set)->keySize);
    //mufHashTableInsertOrAssign(set, node);
}

muf_usize mufHashSetInsertRange(MufHashSet *set, muf_crawptr items, muf_usize count) {
    muf_usize success = 0;
    for (muf_index i = 0; i < count; ++i) {
        success += mufHashSetInsert(set, MUF_RAWPTR_AT(items, _TABLE(set)->keySize, i));
    }
    return success;
}

muf_bool mufHashSetRemove(MufHashSet *set, muf_crawptr item) {
    return mufHashTableRemove(_TABLE(set), item);
}

void mufHashSetClear(MufHashSet *set) {
    mufHashTableClear(_TABLE(set));
}

void mufHashSetRehash(MufHashSet *set, muf_usize newBucketCount) {
    mufHashTableRehash(_TABLE(set), newBucketCount);
}

void mufHashSetForEach(MufHashSet *set, void (*unaryFunc)(muf_rawptr item)) {
    mufHashTableForEach(_TABLE(set), unaryFunc);
}

#undef _CTABLE
#undef _TABLE