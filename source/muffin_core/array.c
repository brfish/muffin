#include "muffin_core/array.h"

#include <stdlib.h>

#include "muffin_core/math.h"

#define MUF_ARRAY_DEFAULT_INIT_CAPACITY     8
#define MUF_ARRAY_DEFAULT_EXPANSION_RATIO   2

MUF_INTERNAL MUF_INLINE void _mufArrayReallocate(MufArray *array, muf_usize newCapacity) {
    array->data = (muf_rawptr) mufReallocate(muf_byte, array->data, array->elementSize * newCapacity);
    array->capacity = newCapacity;
    array->size = mufMin(array->size, newCapacity);
}

MufArray *_mufCreateArray(muf_usize elementSize) {
    MufArray *obj = mufAllocate(MufArray, 1);
    obj->elementSize = elementSize;
    obj->capacity = MUF_ARRAY_DEFAULT_INIT_CAPACITY;
    obj->size = 0;
    obj->data = mufAllocate(muf_byte, obj->capacity * elementSize);
    return obj;
}

void mufInitArray(MufArray *array, const MufArrayConfig *config) {
    array->elementSize = config->elementSize;
    array->capacity = config->initCapacity;
    
    if (config->initDataCount != 0) {
        array->capacity = mufMax(config->initCapacity, config->initDataCount);
        array->data = mufAllocate(muf_byte, array->capacity * array->elementSize);
        array->size = config->initDataCount;
        memcpy(array->data, config->initData, config->initDataCount * config->elementSize);
    } else {
        array->data = mufAllocate(muf_byte, array->capacity * array->elementSize);
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
    MufArray *clone = mufCreateArray(array->elementSize);
}

muf_usize mufArrayCapacity(const MufArray *array) {
    return array->capacity;
}

muf_usize mufArraySize(const MufArray *array) {
    return array->size;
}

muf_bool mufArrayIsEmpty(const MufArray *array) {
    return array->size == 0;
}

void mufArrayInsertRange(MufArray *array, muf_index index, const muf_rawptr elements, muf_usize count) {
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

void mufArrayInsert(MufArray *array, muf_index index, const muf_rawptr element) {
    mufArrayInsertRange(array, index, element, 1);
}

void mufArrayPush(MufArray *array, const muf_rawptr element) {
    if (array->size + 1 > array->capacity)
        _mufArrayReallocate(array, mufMax(array->size + 1, array->capacity << 1));
    
    muf_rawptr copyDst = MUF_RAWPTR_AT(array->data, array->elementSize, array->size);
    memcpy(copyDst, element, array->elementSize);
    ++array->size;
}

void mufArraySet(MufArray *array, muf_index index, const muf_rawptr data) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    muf_rawptr dst = MUF_RAWPTR_AT(array->data, array->elementSize, index);
    memcpy(dst, data, array->elementSize);
}

void mufArrayGet(MufArray *array, muf_index index, muf_rawptr dataOut) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    muf_rawptr src = MUF_RAWPTR_AT(array->data, array->elementSize, index);
    memcpy(dataOut, src, array->elementSize);
}

void mufArrayGetRef(MufArray *array, muf_index index, muf_rawptr *dataRefOut) {
    MUF_FASSERT(index >= 0 && index < array->size, "The index (value is %d) is out of range", index);
    *dataRefOut = MUF_RAWPTR_AT(array->data, array->elementSize, index);
}

void mufArrayGetFront(MufArray *array, muf_rawptr dataOut) {
    mufArrayGet(array, 0, dataOut);
}

void mufArrayGetFrontRef(MufArray *array, muf_rawptr *dataRefOut) {
    mufArrayGetRef(array, 0, dataRefOut);
}

void mufArrayGetBack(MufArray *array, muf_rawptr dataOut) {
    mufArrayGet(array, array->size - 1, dataOut);
}

void mufArrayGetBackRef(MufArray *array, muf_rawptr *dataRefOut) {
    mufArrayGetRef(array, array->size - 1, dataRefOut);
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

void mufArrayShrink(MufArray *array, muf_usize newCapacity) {
    if (newCapacity < array->capacity)
        _mufArrayReallocate(array, newCapacity);
}

void mufArrayReserve(MufArray *array, muf_usize newCapacity) {
    if (newCapacity > array->capacity)
        _mufArrayReallocate(array, newCapacity);
}