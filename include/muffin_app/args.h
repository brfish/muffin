#ifndef _MUFFIN_APP_ARGS_H_
#define _MUFFIN_APP_ARGS_H_

#include "muffin_core/array.h"
#include "muffin_core/common.h"
#include "muffin_core/hash_map.h"

typedef enum MufArgType_e {
    MUF_ARG_TYPE_OPTION,
    MUF_ARG_TYPE_SINGLE_VALUE,
    MUF_ARG_TYPE_MULTI_VALUE,
} MufArgType;

typedef struct MufArgDesc_s {
    const muf_char *name;
    const muf_char *alias;

} MufArgDesc;

typedef struct MufArgsParser_s {
    MufArray *argDescriptors;
} MufArgsParser;

/*

./muffin --forceOutput --input="/image.png" -O baker.png 
两类
    [1] Option
    [2] Key/Value

*/

void mufParseArgs(MufArgsParser *parser, muf_i32 argc, const muf_char **args);



typedef MufHashMap MufArgs;

MufArgs *mufParseArgs(muf_u32 argc, const muf_char **args);
void mufDestroyArgs(MufArgs *pack);

muf_usize mufArgsGetOptionCount(const MufArgs *pack);
muf_usize mufArgsGetPairCount(const MufArgs *pack);

muf_bool mufArgsGetOption(const MufArgs *pack, const muf_char *name);
muf_i32 mufArgsGetInt(const MufArgs *pack, const muf_char *key);
muf_f32 mufArgsGetFloat(const MufArgs *pack, const muf_char *key);
const muf_char *mufArgsGetStr(const MufArgs *pack, const muf_char *key);

#endif