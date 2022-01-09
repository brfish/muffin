#ifndef _MUFFIN_CORE_DICT_H_
#define _MUFFIN_CORE_DICT_H_

#include "muffin_core/common.h"

typedef struct MufDict_s MufDict;

MUF_API MufDict *_mufCreateDict(muf_usize valueSize);

#define mufCreateDict(_valueType) _mufCreateDict(sizeof(_valueType))

MUF_API void mufCloneDict(MufDict *dict);

MUF_API void mufDestroyDict(MufDict *dict);

MUF_API muf_usize mufDictGetSize(const MufDict *dict);

MUF_API muf_bool mufDictIsEmpty(const MufDict *dict);

MUF_API muf_bool mufDictContains(const MufDict *dict, const muf_char *key);

MUF_API muf_rawptr mufDictGetRef(MufDict *dict, const muf_char *key);

MUF_API muf_bool mufDictGet(MufDict *dict, const muf_char *key, muf_rawptr out);

MUF_API muf_rawptr mufDictInsert(MufDict *dict, const muf_char *key, muf_crawptr value);

MUF_API void mufDictInsertOrAssign(MufDict *dict, const muf_char *key, muf_crawptr value);

MUF_API void mufDictPut(MufDict *dict, const muf_char *key, muf_crawptr value);

MUF_API muf_bool mufDictRemove(MufDict *dict, const muf_char *key);

#define mufDictRemoveX(_dict, _key, _destroy) \
    do { muf_rawptr v = mufDictGetRef(_dict, _key); mufDictRemove(_dict, _key); _destroy(v); } while (0)

MUF_API void mufDictClear(MufDict *dict);

MUF_API void mufDictForEach(MufDict *dict, MufBinary2Func(func, void, const muf_char *, muf_rawptr));

#endif