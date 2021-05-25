#ifndef _MUFFIN_CORE_MEMORY_H_
#define _MUFFIN_CORE_MEMORY_H_

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocate a block of memory of size (sizeof(Type) * Count)
 * @param NAME {*}
 */
#define mufAllocate(Type, Count) (Type*)malloc(sizeof(Type) * (Count))
#define mufReallocate(Type, Ptr, NewCount) (Type*)realloc(Ptr, sizeof(Type) * (NewCount))
#define mufAllocateZero(Type, Count) (Type*)calloc(Count, sizeof(Type))

#define mufAllocateAligned(Type, Count, Alignment)

#define mufReallocateAligned(Type, Ptr, NewCount, Alignment)

/**
 * @brief 
 * @param NAME {*}
 * @return {*}
 */
#define mufFree(Ptr) free(Ptr);

/**
 * @brief Free memory safely. If the given pointer is NULL, do nothing.
 * @param Ptr The pointer to a block of memory
 */
#define mufSafeFree(Ptr) do { if (Ptr) { free(Ptr); Ptr = NULL; } } while(0)

#define mufMemoryFill(Ptr, ByteValue, Length) memset(Ptr, ByteValue, Length)

#endif