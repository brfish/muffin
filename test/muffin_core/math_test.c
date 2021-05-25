#include "muffin_core/math.h"

muf_i32 main(muf_i32 argc, const muf_char *argv[]) {
    MufVec2 v1 = { 1, 2 }, v2 = { 2, 3 };
    MufVec2 v3 = mufVec2Add(v1, v2);
    printf("%f %f\n", v3.x, v3.y);
}