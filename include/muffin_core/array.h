#ifndef _MUFFIN_CORE_ARRAY_H_
#define _MUFFIN_CORE_ARRAY_H_

#include "muffin_core/common.h"
#include "muffin_core/memory.h"

typedef struct MufArray_s {
    muf_usize elementSize;
    muf_usize capacity;
    muf_usize size;
    muf_rawptr data;
} MufArray;

typedef struct MufArrayConfig_s {
    muf_usize elementSize;
    muf_usize initCapacity;
    muf_usize initDataCount;
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

MUF_API muf_usize mufArrayCapacity(const MufArray *array);

/**
 * @brief Get the size of the array
 * @param[in] array The array object
 * @return The size of the array
 */
MUF_API muf_usize mufArraySize(const MufArray *array);

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
MUF_API muf_rawptr mufArrayData(MufArray *array);

MUF_API void mufArrayInsertRange(MufArray *array, muf_index index, const muf_rawptr elements, muf_usize count);

MUF_API void mufArrayInsert(MufArray *array, muf_index index, const muf_rawptr element);

MUF_API void mufArrayPush(MufArray *array, const muf_rawptr element);

MUF_API void mufArraySet(MufArray *array, muf_index index, const muf_rawptr data);

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
MUF_API void mufArrayGetRef(MufArray *array, muf_index index, muf_rawptr *dataRefOut);

MUF_API void mufArrayGetFront(MufArray *array, muf_rawptr dataOut);

MUF_API void mufArrayGetFrontRef(MufArray *array, muf_rawptr *dataRefOut);

MUF_API void mufArrayGetBack(MufArray *array, muf_rawptr dataOut);

MUF_API void mufArrayGetBackRef(MufArray *array, muf_rawptr *dataRefOut);

MUF_API void mufArrayRemove(MufArray *array, muf_index index);

MUF_API void mufArrayRemoveX(MufArray *array, muf_index index, const MufDestructor destroy);

MUF_API void mufArrayRemoveRange(MufArray *array, muf_index first, muf_index last);

MUF_API void mufArrayRemoveRangeX(MufArray *array, muf_index index, const MufDestructor destroy);

MUF_API void mufArrayPop(MufArray *array);

#define mufArrayPopX(Array, Destroy) \
    do { muf_rawptr ref; mufArrayGetRef(Array, 0, &ref); Destroy(ref); mufArrayPop(Array); } while (0)

MUF_API void mufArrayClear(MufArray *array);

MUF_API void mufArrayShrink(MufArray *array, muf_usize newCapacity);

MUF_API void mufArrayReserve(MufArray *array, muf_usize newCapacity);


typedef struct MufArrayIterator {
    
} MufArrayIterator;

MUF_API MufArrayIterator mufArrayIteratorBind(MufArray *array);

MUF_API MufArrayIterator mufArrayIteratorNext(MufArrayIterator it);

MUF_API muf_bool mufArrayIteratorHasNext(MufArrayIterator it);

#if defined(__GNUC__)
#   define mufArrayPush_L(Array, Element) \
        do { __typeof__(Element) tmp = Element; mufArrayPush(Array, &tmp); } while(0)
#   define mufArraySet_L(Array, Index, Element) \
        do { __typeof__(Element) tmp = Element; mufArraySet(Array, Index, &tmp); } while(0)
#endif

/*struct MufArrays {
    MufArray *(*create)(muf_usize elementSize);
} MufArrays = {
    _mufCreateArray
};*/

#endif