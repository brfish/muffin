#ifndef _MUFFIN_CORE_ARRAY_H_
#define _MUFFIN_CORE_ARRAY_H_

#include "muffin_core/common.h"
#include "muffin_core/memory.h"

typedef struct MufArray_s {
    muf_usize  elementSize;
    muf_usize  capacity;
    muf_usize  size;
    muf_rawptr data;
} MufArray;

typedef struct MufArrayConfig_s {
    muf_usize  elementSize;
    muf_usize  initCapacity;
    muf_usize  initDataCount;
    muf_rawptr initData;
} MufArrayConfig;

MUF_API MufArray *_mufCreateArray(muf_usize elementSize);

/**
 * @brief Create a dynamic array
 * @tparam _type The type of the array
 * @return The array object
 */
#define mufCreateArray(_type) _mufCreateArray(sizeof(_type))

MUF_API void mufInitArray(MufArray *array, const MufArrayConfig *config);

MUF_API void mufDestroyArray(MufArray *array);

MUF_API MufArray *mufCloneArray(const MufArray *array);

/**
 * @brief Get the capacity of the array
 * @param[in] array Array object
 * @return Capacity of the array
 */
MUF_API muf_usize mufArrayGetCapacity(const MufArray *array);

/**
 * @brief Get the size of the array
 * @param[in] array Array object
 * @return Size of the array
 */
MUF_API muf_usize mufArrayGetSize(const MufArray *array);

/**
 * @brief Check if the array is empty
 * @param[in] array The array object
 * @return True if the array is empty
 */
MUF_API muf_bool mufArrayIsEmpty(const MufArray *array);

/**
 * @brief Get the raw pointer to the underlying data of the array
 * @param[in] array The array object
 * @return The raw pointer to the data
 */
MUF_API muf_rawptr mufArrayGetData(MufArray *array);

MUF_API muf_index mufArrayFind(const MufArray *array, muf_crawptr element, MufEqualityComparator cmp);

MUF_API void mufArrayInsertRange(MufArray *array, muf_index index, muf_crawptr elements, muf_usize count);

MUF_API void mufArrayInsert(MufArray *array, muf_index index, muf_crawptr element);

MUF_API void mufArrayPush(MufArray *array, muf_crawptr element);

MUF_API void mufArraySet(MufArray *array, muf_index index, muf_crawptr data);

/**
 * @brief 
 * @param[in] array The array object
 * @param[in] index The indedx
 * @param[out] dataOut The result
 */
MUF_API void mufArrayGet(MufArray *array, muf_index index, muf_rawptr dataOut);

/**
 * @brief Get a reference or pointer to the element with index.
 * @param[in] array The array object
 * @param[in] index The index of the element that needs to be retrieved
 * @param[out] dataRefOut The pointer to the target
 */
MUF_API muf_rawptr mufArrayGetRef(MufArray *array, muf_index index);

MUF_API muf_crawptr mufArrayGetCRef(const MufArray *array, muf_index index);

MUF_API void mufArrayGetFront(MufArray *array, muf_rawptr dataOut);

MUF_API muf_rawptr mufArrayGetFrontRef(MufArray *array);

MUF_API muf_crawptr mufArrayGetFrontCRef(const MufArray *array);

MUF_API void mufArrayGetBack(MufArray *array, muf_rawptr dataOut);

MUF_API muf_rawptr mufArrayGetBackRef(MufArray *array);

MUF_API muf_crawptr mufArrayGetBackCRef(const MufArray *array);

MUF_API void mufArrayRemove(MufArray *array, muf_index index);

/**
 * @brief Remove an element from the array and call the destructor `_destroy`.
 * @param[in] _array Specify the array.
 * @param[in] _index Specify the index of the element to be removed.
 * @param[in] _destroy Specify the desructor.
 */
#define mufArrayRemoveX(_array, _index, _destroy) \
    do { muf_rawptr ref = mufArrayGetRef(_array, _index); _destroy(ref); mufArrayRemove(_array, _index); } while (0)

/**
 * @brief Remove the elements in the range [first, last).
 * @param[in] array Specify the array.
 * @param[in] first Specify the lower bound of range.
 * @param[in] last Specify the upper bound of range.
 */
MUF_API void mufArrayRemoveRange(MufArray *array, muf_index first, muf_index last);

/**
 * @brief Remove the elements in the range [first, last).
 * @param[in] _array Specify the array.
 * @param[in] _first Specify the lower bound of range.
 * @param[in] _last Specify the upper bound of range.
 * @param[in] _destroy Specify the destructor.
 */
#define mufArrayRemoveRangeX(_array, _first, _last, _destroy) \
    do { \
        for (muf_index i = _first; i < _last; ++i) { \
            muf_rawptr ref = mufArrayGetRef(_array, i); _destroy(ref); \
        } \
        mufArrayRemoveRange(_array, _first, _last); \
    } while (0);

MUF_API void mufArrayPop(MufArray *array);

#define mufArrayPopX(_array, _destroy) \
    do { muf_rawptr ref = mufArrayGetRef(_array, 0); _destroy(ref); mufArrayPop(_array); } while (0)

MUF_API void mufArrayClear(MufArray *array);

#define mufArrayClearX(_array, _destroy) \
    mufArrayRemoveRangeX(_array, 0, _array->size, _destroy)

MUF_API muf_bool mufArrayFindAndRemove(MufArray *array, muf_index first, muf_index last, muf_crawptr target, MufComparator cmp);

MUF_API muf_bool mufArrayFindAndRemoveAll(MufArray *array, muf_index first, muf_index last, muf_crawptr target, MufComparator cmp);

MUF_API void mufArrayShrink(MufArray *array, muf_usize newCapacity);

MUF_API void mufArrayReserve(MufArray *array, muf_usize newCapacity);

MUF_API void mufArrayResize(MufArray *array, muf_usize newSize, muf_crawptr fillElement);

#define mufArrayResizeX(_array, _newSize, _fillElement, _destroy) \
    do { \
        if (_newSize <= _array->size) { \
            for (muf_index i = _newSize; i < _array->size; ++i) _destroy(mufArrayGetRef(_array, i)); \
        } \
        mufArrayResize(_array, _newSize, _fillElement); \
    } while (0)

#define mufArrayForeach(_array, _unaryFunc) \
    do { \
        for (muf_index i = 0; i < _array->size; ++i) { \
            muf_rawptr ref = mufArrayGetRef(_array, i); _unaryFunc(ref); \
        } \
    } while (0)

typedef struct MufArrayIterator {
    
} MufArrayIterator;

MUF_API MufArrayIterator mufArrayIteratorBind(MufArray *array);

MUF_API MufArrayIterator mufArrayIteratorNext(MufArrayIterator it);

MUF_API muf_bool mufArrayIteratorHasNext(MufArrayIterator it);

#if defined(__GNUC__) && defined(MUF_ENABLE_GCC_EXTENSION)
#   define mufArrayInsert_R(_array, _index, Element) \
        do { __typeof__(Element) tmp = Element; mufArrayInsert(_array, _index, &tmp); } while(0)
#   define mufArrayPush_R(_array, Element) \
        do { __typeof__(Element) tmp = Element; mufArrayPush(_array, &tmp); } while(0)
#   define mufArraySet_R(_array, _index, Element) \
        do { __typeof__(Element) tmp = Element; mufArraySet(_array, _index, &tmp); } while(0)
#endif

#endif