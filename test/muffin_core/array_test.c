#include <stdio.h>

#include "muffin_core/array.h"

muf_i32 main(muf_i32 argc, const muf_char *argv[]) {
    MufArray *arr = mufCreateArray(muf_i32);
    
    for (muf_index i = 0; i < 20; ++i) {
        mufArrayPush(arr, &i);
    }
    MUF_FASSERT(mufArraySize(arr) == 20, "UnitTest0 (ArrayInsert) failed: incorrect size");
    MUF_FASSERT(mufArrayCapacity(arr) == 32, "UnitTest0 (ArrayInsert) failed: incorrect capacity");

    for (muf_index i = 0; i < 20; ++i) {
        muf_i32 value;
        mufArrayGet(arr, i, &value);
        MUF_FASSERT(value == i, "UnitTest1 (ArrayGet) failed: incorrect value");
    }

    mufDestroyArray(arr);
    mufDestroyArray(NULL);

    puts("Unit test all passed");
    return 0;
}
