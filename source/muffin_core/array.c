#include "muffin_core/array.h"

#include <stdlib.h>

#include "muffin_core/math.h"

#define MUF_ARRAY_DEFAULT_INIT_CAPACITY     8
#define MUF_ARRAY_DEFAULT_EXPANSION_RATIO   2

typedef struct _internal_MufArray_s {
    muf_usize elementSize;
    muf_usize size;
    muf_usize capacity;
    muf_rawptr data;
} _internal_MufArray;

MUF_INTERNAL MUF_INLINE void _mufArrayReallocate(MufArray *array, muf_usize newCapacity) {
    array->data = (muf_rawptr) mufRealloc(muf_byte, array->data, array->elementSize * newCapacity);
    array->capacity = newCapacity;
    array->size = mufMin(array->size, newCapacity);
}

MufArray *_mufCreateArray(muf_usize elementSize) {
    MufArray *obj = mufAlloc(MufArray, 1);
    obj->elementSize = elementSize;
    obj->capacity = MUF_ARRAY_DEFAULT_INIT_CAPACITY;
    obj->size = 0;
    obj->data = mufAlloc(muf_byte, obj->capacity * elementSize);
    return obj;
}

void mufInitArray(MufArray *array, const MufArrayConfig *config) {
    array->elementSize = config->elementSize;
    array->capacity = config->initCapacity;
    
    if (config->initDataCount != 0) {
        array->capacity = mufMax(config->initCapacity, config->initDataCount);
        array->data = mufAlloc(muf_byte, array->capacity * array->elementSize);
        array->size = config->initDataCount;
        memcpy(array->data, config->initData, config->initDataCount * config->elementSize);
    } else {
        array->data = mufAlloc(muf_byte, array->capacity * array->elementSize);
        array->size = 0;
    }
}

void mufDestroyArray(MufArray *array) {
    MUF_FCHECK(array != NULL, "Trying to release null pointer");
    if (array == NULL)
        return;
    mufSafeFree(array->data);
    mufFree(array);
}

MufArray *mufCloneArray(const MufArray *array) {
    if (array == NULL)
        return NULL;
    MufArray *clone = _mufCreateArray(array->elementSize);
    mufArrayReserve(clone, array->capacity);
    memcpy(clone->data, array->data, array->elementSize * array->size);
    clone->size = array->size;
    return clone;
}

muf_usize mufArrayGetCapacity(const MufArray *array) {
    return array->capacity;
}

muf_usize mufArrayGetSize(const MufArray *array) {
    return array->size;
}

muf_bool mufArrayIsEmpty(const MufArray *array) {
    return array->size == 0;
}

muf_rawptr mufArrayGetData(MufArray *array) {
    return array->data;
}

muf_index mufArrayFind(const MufArray *array, muf_crawptr element, MufEqualityComparator cmp) {
    for (muf_index i = 0; i < array->size; ++i) {
        muf_crawptr ref = mufArrayGetCRef(array, i);
        if (cmp(ref, element) == MUF_TRUE) {
            return i;
        }
    }
    return (muf_index) -1;
}

void mufArrayInsertRange(MufArray *array, muf_index index, muf_crawptr elements, muf_usize count) {
    MUF_FASSERT(index >= 0 && index <= array->size, "The index (value is %d) is out of range", index);

    if (array->size + count > array->capacity)
        _mufArrayReallocate(array, mufMax(array->size + count, array->capacity << 1));
    
    muf_rawptr moveDst = MUF_RAWPTR_AT(array->data, array->elementSize, index + count);
    muf_rawptr moveSrc = MUF_RAWPTR_AT(array->data, array->elementSize, index);
    muf_rawptr copyDst = moveSrc;
    memmove(moveDst, moveSrc, array->size - index);
    memcpy(copyDst, elements, array->elementSize * count);
    array->size += count;
}

void mufArrayInsert(MufArray *array, muf_index index, muf_crawptr element) {
    mufArrayInsertRange(array, index, element, 1);
}

void mufArrayPush(MufArray *array, muf_crawptr element) {
    if (array->size + 1 > array->capacity)
        _mufArrayReallocate(array, mufMax(array->size + 1, array->capacity << 1));
    
    muf_rawptr copyDst = MUF_RAWPTR_AT(array->data, array->elementSize, array->size);
    memcpy(copyDst, element, array->elementSize);
    ++array->size;
}

void mufArraySet(MufArray *array, muf_index index, muf_crawptr data) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    muf_rawptr dst = MUF_RAWPTR_AT(array->data, array->elementSize, index);
    memcpy(dst, data, array->elementSize);
}

void mufArrayGet(MufArray *array, muf_index index, muf_rawptr dataOut) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    muf_rawptr src = MUF_RAWPTR_AT(array->data, array->elementSize, index);
    memcpy(dataOut, src, array->elementSize);
}

muf_rawptr mufArrayGetRef(MufArray *array, muf_index index) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    return MUF_RAWPTR_AT(array->data, array->elementSize, index);
}

muf_crawptr mufArrayGetCRef(const MufArray *array, muf_index index) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    return MUF_RAWPTR_AT(array->data, array->elementSize, index);
}

void mufArrayGetFront(MufArray *array, muf_rawptr dataOut) {
    mufArrayGet(array, 0, dataOut);
}

muf_rawptr mufArrayGetFrontRef(MufArray *array) {
    return mufArrayGetRef(array, 0);
}

muf_crawptr mufArrayGetFrontCRef(const MufArray *array) {
    return mufArrayGetCRef(array, 0);
}

void mufArrayGetBack(MufArray *array, muf_rawptr dataOut) {
    mufArrayGet(array, array->size - 1, dataOut);
}

muf_rawptr mufArrayGetBackRef(MufArray *array) {
    return mufArrayGetRef(array, array->size - 1);
}

muf_crawptr mufArrayGetBackCRef(const MufArray *array) {
    return mufArrayGetCRef(array, array->size - 1);
}

void mufArrayRemove(MufArray *array, muf_index index) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);

    if (index == array->size - 1) {
        --array->size;
        return;
    }
    muf_rawptr moveDst = MUF_RAWPTR_AT(array->data, array->elementSize, index);
    muf_rawptr moveSrc = MUF_RAWPTR_AT(array->data, array->elementSize, index + 1);
    memmove(moveDst, moveSrc, array->elementSize * (array->size - index));
    --array->size;
}

void mufArrayRemoveRange(MufArray *array, muf_index first, muf_index last) {
    MUF_FASSERT(first <= last && first >= 0 && last <= array->size, "The given range ([%d,%d)) is out", first, last);

    if (first == last)
        return;
    if (last == array->size) {
        array->size -= last - first;
        return;
    }

    muf_rawptr moveDst = MUF_RAWPTR_AT(array->data, array->elementSize, first);
    muf_rawptr moveSrc = MUF_RAWPTR_AT(array->data, array->elementSize, last);
    memmove(moveDst, moveSrc, array->elementSize * (array->size - last));
    array->size -= last - first;
}

void mufArrayPop(MufArray *array) {
    --array->size;
}

void mufArrayClear(MufArray *array) {
    array->size = 0;
}

muf_bool mufArrayFindAndRemove(MufArray *array, muf_index first, muf_index last, muf_crawptr target, MufComparator cmp) {
    for (muf_index i = last - 1; i >= first; --i) {
        muf_rawptr ref = mufArrayGetRef(array, i);
        if (cmp(ref, target)) {
            mufArrayRemove(array, i);
            return MUF_TRUE;
        }
    }
    return MUF_FALSE;
}

muf_bool mufArrayFindAndRemoveAll(MufArray *array, muf_index first, muf_index last, muf_crawptr target, MufComparator cmp) {
    muf_bool found = MUF_FALSE;
    for (muf_index i = last - 1; i >= first; --i) {
        muf_rawptr ref = mufArrayGetRef(array, i);
        if (cmp(ref, target)) {
            mufArrayRemove(array, i);
            found = MUF_TRUE;
        }
    }
    return found;
}

void mufArrayShrink(MufArray *array, muf_usize newCapacity) {
    if (newCapacity < array->capacity)
        _mufArrayReallocate(array, newCapacity);
}

void mufArrayReserve(MufArray *array, muf_usize newCapacity) {
    if (newCapacity > array->capacity)
        _mufArrayReallocate(array, newCapacity);
}

void mufArrayResize(MufArray *array, muf_usize newSize, muf_crawptr fillElement) {
    if (newSize <= array->size) {
        array->size = newSize;
        return;
    }

    if (newSize > array->capacity) {
        mufArrayReserve(array, newSize);
    }

    for (muf_index i = array->size; i < newSize; ++i) {
        mufArrayPush(array, fillElement);
    }
}