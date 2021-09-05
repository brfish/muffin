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
 * @tparam Type The type of the array
 * @return The array object
 */
#define mufCreateArray(Type) _mufCreateArray(sizeof(Type))

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
 * @brief Remove an element from the array and call the destructor `Destroy`.
 * @param[in] Array Specify the array.
 * @param[in] Index Specify the index of the element to be removed.
 * @param[in] Destroy Specify the desructor.
 */
#define mufArrayRemoveX(Array, Index, Destroy) \
    do { muf_rawptr ref = mufArrayGetRef(Array, Index); Destroy(ref); mufArrayRemove(Array, Index); } while (0)

/**
 * @brief Remove the elements in the range [first, last).
 * @param[in] array Specify the array.
 * @param[in] first Specify the lower bound of range.
 * @param[in] last Specify the upper bound of range.
 */
MUF_API void mufArrayRemoveRange(MufArray *array, muf_index first, muf_index last);

/**
 * @brief Remove the elements in the range [first, last).
 * @param[in] Array Specify the array.
 * @param[in] First Specify the lower bound of range.
 * @param[in] Last Specify the upper bound of range.
 * @param[in] Destroy Specify the destructor.
 */
#define mufArrayRemoveRangeX(Array, First, Last, Destroy) \
    do { \
        for (muf_index i = First; i < Last; ++i) { \
            muf_rawptr ref = mufArrayGetRef(Array, i); Destroy(ref); \
        } \
        mufArrayRemoveRange(Array, First, Last); \
    } while (0);

MUF_API void mufArrayPop(MufArray *array);

#define mufArrayPopX(Array, Destroy) \
    do { muf_rawptr ref = mufArrayGetRef(Array, 0); Destroy(ref); mufArrayPop(Array); } while (0)

MUF_API void mufArrayClear(MufArray *array);

#define mufArrayClearX(Array, Destroy) \
    mufArrayRemoveRangeX(Array, 0, Array->size, Destroy)

MUF_API muf_bool mufArrayFindAndRemove(MufArray *array, muf_index first, muf_index last, muf_crawptr target, MufComparator cmp);

MUF_API muf_bool mufArrayFindAndRemoveAll(MufArray *array, muf_index first, muf_index last, muf_crawptr target, MufComparator cmp);

MUF_API void mufArrayShrink(MufArray *array, muf_usize newCapacity);

MUF_API void mufArrayReserve(MufArray *array, muf_usize newCapacity);

MUF_API void mufArrayResize(MufArray *array, muf_usize newSize, muf_crawptr fillElement);

#define mufArrayResizeX(Array, NewSize, FillElement, Destroy) \
    do { \
        if (NewSize <= Array->size) { \
            for (muf_index i = NewSize; i < Array->size; ++i) Destroy(mufArrayGetRef(Array, i)); \
        } \
        mufArrayResize(Array, NewSize, FillElement); \
    } while (0)

#define mufArrayForeach(Array, UnaryFunc) \
    do { \
        for (muf_index i = 0; i < Array->size; ++i) { \
            muf_rawptr ref = mufArrayGetRef(Array, i); UnaryFunc(ref); \
        } \
    } while (0)

typedef struct MufArrayIterator {
    
} MufArrayIterator;

MUF_API MufArrayIterator mufArrayIteratorBind(MufArray *array);

MUF_API MufArrayIterator mufArrayIteratorNext(MufArrayIterator it);

MUF_API muf_bool mufArrayIteratorHasNext(MufArrayIterator it);

#if defined(__GNUC__) && defined(MUF_ENABLE_GCC_EXTENSION)
#   define mufArrayInsert_R(Array, Index, Element) \
        do { __typeof__(Element) tmp = Element; mufArrayInsert(Array, Index, &tmp); } while(0)
#   define mufArrayPush_R(Array, Element) \
        do { __typeof__(Element) tmp = Element; mufArrayPush(Array, &tmp); } while(0)
#   define mufArraySet_R(Array, Index, Element) \
        do { __typeof__(Element) tmp = Element; mufArraySet(Array, Index, &tmp); } while(0)
#endif

#endif