#ifndef _MUFFIN_CORE_MEMORY_H_
#define _MUFFIN_CORE_MEMORY_H_

#include <stdlib.h>
#include <string.h>

#include "muffin_core/common.h"

typedef muf_rawptr(* MufAllocCallback)(muf_usize size, muf_rawptr userData);
typedef void(* MufDeallocCallback)(muf_rawptr ptr, muf_rawptr userData);

typedef struct MufAllocator_s {
    MufAllocCallback alloc;
    MufDeallocCallback dealloc;
} MufAllocator;

#define mufAlignOf()

/**
 * @brief Allocate a block of memory of size (sizeof(Type) * Count)
 * @param Type The data type to be allocated
 * @param Count The count of elements
 */
#define mufAlloc(Type, Count) (Type*)malloc(sizeof(Type) * (muf_usize)(Count))
#define mufRealloc(Type, Ptr, NewCount) (Type*)realloc(Ptr, sizeof(Type) * (muf_usize)(NewCount))
#define mufAllocZero(Type, Count) (Type*)calloc(Count, sizeof(Type))

#define mufAllocBytes(Count) mufAlloc(muf_byte, Count)
#define mufReallocBytes(Ptr, NewCount) mufRealloc(muf_byte, Ptr, NewCount)

#define mufAllocAligned(Type, Count, Alignment)

#define mufReallocAligned(Type, Ptr, NewCount, Alignment)

#define mufAllocZeroAligned(Type, Count, Alignment)

/**
 * @brief Free memory for the given pointer
 * @param Ptr The pointer to a block of memory
 */
#define mufFree(Ptr) free(Ptr);

/**
 * @brief Free memory safely. If the given pointer is NULL, do nothing.
 * @param Ptr The pointer to a block of memory
 */
#define mufSafeFree(Ptr) do { if (Ptr) { free(Ptr); Ptr = NULL; } } while(0)

/**
 * @brief Fill the memory with the given byte value
 * @param Ptr Specify the pointer to a block of memory
 * @param ByteValue Specify the value to be filled
 * @param Length Specify the length of the memory
 */
#define mufMemFill(Ptr, ByteValue, Length) memset(Ptr, ByteValue, Length)

#define mufMemCopy(Dst, Src, Type, Count) memcpy(Dst, Src, sizeof(Type) * (muf_usize)(Count))

#define mufMemCopyBytes(Dst, Src, Count) memcpy(Dst, Src, Count);

#define mufMemMove(Dst, Src, Type, Count) memmove(Dst, Src, sizeof(Type) * (muf_usize)(Count))

#define mufMemMoveBytes(Dst, Src, Count) memmove(Dst, Src, Count);

#define mufMemCompare(Mem0, Mem1, Size) (memcmp(Mem0, Mem1, (Size)))

#define mufMemEqual(Mem0, Mem1, Size) (mufMemCompare(Mem0, Mem1, Size) == 0)

#endif