#ifndef _MUFFIN_CORE_MEMORY_H_
#define _MUFFIN_CORE_MEMORY_H_

#include <stdlib.h>
#include <string.h>

#include "muffin_core/common.h"

typedef MufUnaryFunc(MufAllocCallback, muf_rawptr, muf_usize);
typedef MufUnaryFunc(MufDeallocCallback, void, muf_rawptr);
typedef MufBinary2Func(MufReallocCallback, muf_rawptr, muf_rawptr, muf_usize);

typedef struct MufAllocatorCallbacks_s {
    MufAllocCallback    alloc;
    MufDeallocCallback  dealloc;
    MufReallocCallback  realloc;
} MufAllocatorCallbacks;

extern MufAllocatorCallbacks MUF_DEFAULT_ALLOCATOR[1];

#define mufAlignOf()

/**
 * @brief Allocate a block of memory of size (sizeof(_type) * _count)
 * @param _type The data type to be allocated
 * @param _count The count of elements
 */
#define mufAlloc(_type, _count) (_type*)malloc(sizeof(_type) * (muf_usize)(_count))
#define mufRealloc(_type, _ptr, _newCount) (_type*)realloc(_ptr, sizeof(_type) * (muf_usize)(_newCount))
#define mufAllocZero(_type, _count) (_type*)calloc(_count, sizeof(_type))

#define mufAllocBytes(_count) mufAlloc(muf_byte, _count)
#define mufReallocBytes(_ptr, _newCount) mufRealloc(muf_byte, _ptr, _newCount)

#define mufAllocAligned(_type, _count, Alignment)

#define mufReallocAligned(_type, _ptr, _newCount, Alignment)

#define mufAllocZeroAligned(_type, _count, Alignment)

/**
 * @brief Free memory for the given pointer
 * @param _ptr The pointer to a block of memory
 */
#define mufFree(_ptr) free(_ptr);

/**
 * @brief Free memory safely. If the given pointer is NULL, do nothing.
 * @param _ptr The pointer to a block of memory
 */
#define mufSafeFree(_ptr) do { if (_ptr) { free(_ptr); _ptr = NULL; } } while(0)

/**
 * @brief Fill the memory with the given byte value
 * @param _ptr Specify the pointer to a block of memory
 * @param _byteValue Specify the value to be filled
 * @param _length Specify the length of the memory
 */
#define mufMemFill(_ptr, _byteValue, _length) memset(_ptr, _byteValue, _length)

#define mufMemCopy(_dst, _src, _type, _count) memcpy(_dst, _src, sizeof(_type) * (muf_usize)(_count))

#define mufMemCopyBytes(_dst, _src, _count) memcpy(_dst, _src, _count);

#define mufMemMove(_dst, _src, _type, _count) memmove(_dst, _src, sizeof(_type) * (muf_usize)(_count))

#define mufMemMoveBytes(_dst, _src, _count) memmove(_dst, _src, _count);

#define mufMemCompare(_ptr0, _ptr1, _size) (memcmp(_ptr0, _ptr1, (_size)))

#define mufMemEqual(_ptr0, _ptr1, _size) (mufMemCompare(_ptr0, _ptr1, _size) == 0)

#endif