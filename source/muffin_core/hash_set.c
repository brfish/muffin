#include "muffin_core/hash_set.h"

#include "muffin_core/memory.h"

MUF_INTERNAL muf_rawptr _mufHashSetKeyExtract(MufHashTableNode *node) {
    return MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashSetNode), 1);
}

MufHashSet *_mufCreateHashSet(muf_usize elementSize, MufEqualityComparator equal, MufHash hash) {
    return mufCreateHashTable(elementSize, 0, _mufHashSetKeyExtract, equal, hash);
}

void mufDestroyHashSet(MufHashSet *set) {
    mufDestroyHashTable(set);
}

MufHashSet *mufCloneHashSet(const MufHashSet *set) {
    MufHashSet *clone = _mufCreateHashSet(set->keySize, set->equal, set->hash);
    
}

muf_usize mufHashSetSize(const MufHashSet *set) {
    return set->size;
}

muf_usize mufHashSetBucketCount(const MufHashSet *set) {
    return set->bucketCount;
}

muf_bool mufHashSetIsEmpty(const MufHashSet *set) {
    return set->size == 0;
}

muf_f32 mufHashSetGetLoadFactor(const MufHashSet *set) {
    return mufHashTableGetLoadFactor(set);
}

MufHashSetNode *mufHashSetFind(MufHashSet *set, muf_rawptr item) {
    return mufHashTableFind(set, item, NULL);
}

muf_bool mufHashSetContains(const MufHashSet *set, muf_rawptr item) {
    return mufHashSetFind((MufHashSet *)set, item) != NULL;
}

void mufHashSetInsert(MufHashSet *set, muf_rawptr item) {
    MufHashSetNode *node = (MufHashSetNode *) mufAllocate(muf_byte, sizeof(MufHashSetNode) + set->keySize);
    node->next = NULL;
    node->hashCode = set->hash(item);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashSetNode), 1), item, set->keySize);
    mufHashTableInsert(set, node);
}

void mufHashSetInsertOrAssign(MufHashSet *set, muf_rawptr item) {
    MufHashSetNode *node = (MufHashSetNode *) mufAllocate(muf_byte, sizeof(MufHashSetNode) + set->keySize);
    node->next = NULL;
    node->hashCode = set->hash(item);
    memcpy(MUF_RAWPTR_AT((muf_rawptr) node, sizeof(MufHashSetNode), 1), item, set->keySize);
    mufHashTableInsertOrAssign(set, node);
}

void mufHashSetInsertRange(MufHashSet *set, muf_rawptr items, muf_usize count) {
    for (muf_index i = 0; i < count; ++i) {
        mufHashSetInsert(set, MUF_RAWPTR_AT(items, set->keySize, i));
    }
}

void mufHashSetRemove(MufHashSet *set, muf_rawptr item) {
    mufHashTableRemove(set, item);
}

void mufHashSetClear(MufHashSet *set) {
    mufHashTableClear(set);
}

void mufHashSetRehash(MufHashSet *set, muf_usize newBucketCount) {
    mufHashTableRehash(set, newBucketCount);
}