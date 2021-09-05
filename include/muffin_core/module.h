#ifndef _MUFFIN_CORE_MODULE_H_
#define _MUFFIN_CORE_MODULE_H_

#include "muffin_core/common.h"

#define MUF_MODULE_REGISTRY_DECLARE(ModuleName) extern MufModuleRegistry _muf_module_##ModuleName[1]

#define MUF_MODULE_REGISTRY_DEF(ModuleName, IdentifyName, Description, Version, Priority, Load, Unload) \
    MufModuleRegistry _muf_module_##ModuleName[1] = {{ \
        .name = IdentifyName, \
        .description = Description, \
        .version = Version, \
        .priority = Priority, \
        .loadCallback = Load, \
        .unloadCallback = Unload \
    }}

enum {
    MUF_MODULE_NAME_MAX_LEN = 256,
    MUF_MODULE_DESCRIPTION_MAX_LEN = 512
};

typedef void (*MufModuleLoadCallback)(void);
typedef void (*MufModuleUnloadCallback)(void);

typedef struct MufModuleRegistry_s {
    const muf_char          *name;
    const muf_char          *description;
    MufVersion              version;
    muf_u32                 priority;
    MufModuleLoadCallback   loadCallback;
    MufModuleUnloadCallback unloadCallback;
} MufModuleRegistry;

enum MufResult_Module {
    MUF_RESULT_MODULE_EXISTED,
    MUF_RESULT_MODULE_FAILURE
};

typedef enum MufModuleStatus_e {
    MUF_MODULE_STATUS_READY,
    MUF_MODULE_STATUS_LOADED,
    MUF_MODULE_STATUS_ERROR
} MufModuleStatus;

typedef enum MufModuleEvent_e {
    MUF_MODULE_EVENT_LOAD,
    MUF_MODULE_EVENT_UNLOAD,
    MUF_MODULE_EVENT_FATAL_ERROR,
} MufModuleEvent;

MUF_HANDLE_DEF(MufModule);

void mufRegisterModule(const MufModuleRegistry *registry);
MufModule mufGetModule(const muf_char *modName);
muf_usize mufGetModuleNames(const muf_char **names);
muf_usize mufGetEnabledModuleCount();

void mufLoadModule(MufModule mod);
void mufUnloadModule(MufModule mod);
void mufReloadModule(MufModule mod, muf_bool force);

const muf_char *mufModuleGetName(MufModule mod);
const muf_char *mufModuleGetDescription(MufModule mod);
MufVersion mufModuleGetVersion(MufModule mod);
muf_u32 mufModuleGetPriority(MufModule mod);
MufModuleStatus mufModuleGetStatus(MufModule mod);

void mufModuleSetPriority(MufModule mod, muf_u32 priority);
void mufModuleReportError(MufModule mod, const muf_char *format, ...);

#endif