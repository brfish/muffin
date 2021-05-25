#include "muffin_core/hash_map.h"
#include "muffin_core/hash_set.h"
#include "muffin_core/hash.h"

muf_i32 main(muf_i32 argc, const muf_char *argv[]) {
    printf("String HashValue: %u\n", mufHash_str("Hello World"));
    printf("Float HashValue: %u\n", mufHash_f32((float []){ 132.555F }));

    MufHashSet *s = mufCreateHashSet_i32();

    printf("size: %d, bucketCount: %d\n", s->size, s->bucketCount);

    mufHashSetInsert_L(s, 956);
    printf("after, size: %d, bucketCount: %d\n", s->size, s->bucketCount);
    printf("contains: %s\n", mufHashSetContains_L(s, 237) ? "true" : "false");

    mufDestroyHashSet(s);

    MufHashMap *m = mufCreateHashMap(muf_i32, muf_f32, mufEqual_i32, mufHash_i32);

    mufHashMapInsert_L(m, 10603, 114.514F);
    printf("contains: %s\n", mufHashMapContains_L(m, 10603) ? "true" : "false");

    mufHashMapInsert_L(m, 2333, 6666.0F);

    muf_f32 value;
    mufHashMapGet_L(m, 10603, &value);
    printf("value: %f\n", value);
    mufHashMapGet_L(m, 2333, &value);
    printf("value: %f\n", value);

    //mufHashMapRemove_L(m, 10603);
    printf("contains: %s\n", mufHashMapContains_L(m, 10603) ? "true" : "false");
    printf("size: %d, bucketCount: %d\n", m->size, m->bucketCount);

    mufDestoryHashMap(m);
}