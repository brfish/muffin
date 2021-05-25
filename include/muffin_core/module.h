#ifndef _MUFFIN_CORE_MODULE_H_
#define _MUFFIN_CORE_MODULE_H_

#include "muffin_core/common.h"

#define MUF_MODULE_NAME_MAX_LEN 256

typedef struct MufModule_s {
    muf_char name[MUF_MODULE_NAME_MAX_LEN];
} MufModule;

MufModule *mufLoadModule();

#endif