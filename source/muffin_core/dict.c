#include "muffin_core/dict.h"

#include "internal/hash_table.h"
#include "muffin_core/memory.h"
#include "muffin_core/string.h"

#define _TABLE(Ptr) ((MufHashTable *)(Ptr))

typedef struct _MufStrWrapper_s {
    muf_usize length;
    muf_char  *data;
} _MufStrWrapper;

static muf_rawptr _mufDictKeyExtract(MufHashTableNode *node) {
    return mufHashTableNodeGetData(node);
}

static muf_bool _mufStrWrapperEqual(muf_crawptr a, muf_crawptr b) {
    const _MufStrWrapper *s1 = (const _MufStrWrapper *) a;
    const _MufStrWrapper *s2 = (const _MufStrWrapper *) b;
    return s1->data == s2->data ? MUF_TRUE : 
        (s1->length == s2->length ? mufCStrCompare(a, b) == 0 : MUF_FALSE);
}

static muf_index _mufStrWrapperHash(muf_crawptr str) {
    _MufStrWrapper *s = (_MufStrWrapper *) str;
    return mufHashBytes(s->data, s->length);
}

MufDict *_mufCreateDict(muf_usize valueSize) {
    MufDict *dict = (MufDict *) mufCreateHashTable(sizeof(_MufStrWrapper), valueSize, 
        _mufDictKeyExtract, _mufStrWrapperEqual, _mufStrWrapperHash);
    return dict;
}

void mufDestroyDict(MufDict *dict) {
    mufDictClear(dict);
    mufFree(_TABLE(dict)->buckets);
    mufFree(dict);
}

void mufCloneDict(MufDict *dict) {
    MufDict *clone = _mufCreateDict(_TABLE(dict)->valueSize);
    (void) clone;
}

muf_usize mufDictGetSize(const MufDict *dict) {
    return _TABLE(dict)->size;
}

muf_bool mufDictIsEmpty(const MufDict *dict) {
    return _TABLE(dict)->size == 0;
}

muf_bool mufDictContains(const MufDict *dict, const muf_char *key) {
    _MufStrWrapper query;
    query.data = (muf_char *) key;
    query.length = mufCStrLength(key);
    return mufHashTableFind(_TABLE(dict), &query, NULL) != NULL;
}

muf_rawptr mufDictGetRef(MufDict *dict, const muf_char *key) {
    _MufStrWrapper query;
    query.data = (muf_char *) key;
    query.length = mufCStrLength(key);
    MufHashTableNode *node = mufHashTableFind(_TABLE(dict), &query, NULL);
    return node == NULL ? NULL : mufHashTableExtractNodeValue(_TABLE(dict), node);
}

muf_bool mufDictGet(MufDict *dict, const muf_char *key, muf_rawptr out) {
    _MufStrWrapper query;
    query.data = (muf_char *) key;
    query.length = mufCStrLength(key);
    MufHashTableNode *node = mufHashTableFind(_TABLE(dict), &query, NULL);

    if (!node) { 
        return MUF_FALSE;
    }

    muf_rawptr value = mufHashTableExtractNodeValue(_TABLE(dict), node);
    mufMemCopyBytes(out, value, _TABLE(dict)->valueSize);
    return MUF_TRUE;
}

static MUF_INLINE MufHashTableNode *_mufDictCreateNode(const MufDict *dict, const muf_char *key, muf_crawptr value) {
    MufHashTableNode *node = mufCreateHashTableNode(NULL, 0, _TABLE(dict)->storageSize, NULL);
    _MufStrWrapper *keyWrapper = (_MufStrWrapper *) mufHashTableExtractNodeKey(_TABLE(dict), node);
    keyWrapper->data = mufCStrClone(key);
    keyWrapper->length = mufCStrLength(key);
    node->hashCode = _mufStrWrapperHash(keyWrapper);
    muf_rawptr valueDst = mufHashTableExtractNodeValue(_TABLE(dict), node);
    mufMemCopyBytes(valueDst, value, _TABLE(dict)->valueSize);
    return node;
}

muf_rawptr mufDictInsert(MufDict *dict, const muf_char *key, muf_crawptr value) {
    if (mufDictContains(dict, key)) {
        return NULL;
    }

    MufHashTableNode *node = _mufDictCreateNode(dict, key, value);
    mufHashTableInsert(_TABLE(dict), node);
    return mufHashTableExtractNodeValue(_TABLE(dict), node);
}

void mufDictInsertOrAssign(MufDict *dict, const muf_char *key, muf_crawptr value) {
    muf_rawptr ref = mufDictGetRef(dict, key);
    if (ref) {
        mufMemCopyBytes(ref, value, _TABLE(dict)->valueSize);
    } else {
        MufHashTableNode *node = _mufDictCreateNode(dict, key, value);
        mufHashTableInsert(_TABLE(dict), node);
    }
}

void mufDictPut(MufDict *dict, const muf_char *key, muf_crawptr value) {
    mufDictInsertOrAssign(dict, key, value);
}

muf_bool mufDictRemove(MufDict *dict, const muf_char *key) {
    MufHashTable *table = _TABLE(dict);
    _MufStrWrapper query;
    query.data = (muf_char *) key;
    query.length = mufCStrLength(key);

    MufHashTableNode *prev = NULL;
    MufHashTableNode *node = mufHashTableFind(table, &query, &prev);
    if (!node) {
        return MUF_FALSE;
    }
    _MufStrWrapper *keyWrapper = mufHashTableExtractNodeKey(table, node);
    mufFree(keyWrapper->data);

    if (prev) {
        prev->next = node->next;
    } else {
        muf_index bucketIndex = node->hashCode & (table->bucketCount - 1);
        table->buckets[bucketIndex] = node->next;
    }
    --table->size;
    mufFree(node);
    return MUF_TRUE;
}

void mufDictClear(MufDict *dict) {
    MufHashTable *table = _TABLE(dict);
    for (muf_index i = 0; i < table->bucketCount; ++i) {
        MufHashTableNode *node = table->buckets[i];
        while (node) {
            _MufStrWrapper *wrapper = (_MufStrWrapper *) _mufDictKeyExtract(node);
            mufFree(wrapper->data);
            MufHashTableNode *next = node->next;
            mufFree(node);
            node = next;
        }
    }
    table->size = 0;
}

void mufDictForEach(MufDict *dict, MufBinary2Func(func, void, const muf_char *, muf_rawptr)) {
    MufHashTable *table = _TABLE(dict);
    for (muf_index i = 0; i < table->bucketCount; ++i) {
        MufHashTableNode *node = table->buckets[i];
        while (node) {
            MufHashTableNode *next = node->next;
            _MufStrWrapper *key = (_MufStrWrapper *) mufHashTableExtractNodeKey(table, node);
            muf_rawptr *value = mufHashTableExtractNodeValue(table, node);
            func(key->data, value);
            node = next;
        }
    }
}

#undef _TABLE