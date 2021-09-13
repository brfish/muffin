#include "muffin_core/memory.h"

#include <stdlib.h>

static muf_rawptr _mufDefaultAlloc(muf_usize size) {
    return malloc(size);
}

static void _mufDefaultDealloc(muf_rawptr ptr) {
    free(ptr);
}

static muf_rawptr _mufDefaultRealloc(muf_rawptr ptr, muf_usize newSize) {
    return realloc(ptr, newSize);
}

MufAllocatorCallbacks MUF_DEFAULT_ALLOCATOR[1] = {{
    _mufDefaultAlloc,
    _mufDefaultDealloc,
    _mufDefaultRealloc
}};