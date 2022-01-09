#include "muffin_core/module.h"

#include <stdarg.h>
#include <time.h>

#include "muffin_core/array.h"
#include "muffin_core/dict.h"
#include "muffin_core/log.h"
#include "muffin_core/memory.h"
#include "muffin_core/string.h"

typedef struct _MufModule_s {
    muf_char                name[MUF_MODULE_NAME_MAX_LEN];
    muf_char                description[MUF_MODULE_DESCRIPTION_MAX_LEN];
    MufVersion              version;
    muf_u32                 priority;
    MufModuleStatus         status;
    muf_u64                 loadTime;
    MufModuleLoadCallback   loadCallback;
    MufModuleUnloadCallback unloadCallback;
} _MufModule;

typedef struct _MufModuleManager_s {
    MufArray *names;
    MufDict  *modules;
} _MufModuleManager;

static _MufModuleManager *_manager = NULL;

static _MufModuleManager *_mufCreateModuleManager() {
    _MufModuleManager *manager = mufAlloc(_MufModuleManager, 1);
    manager->names = mufCreateArray(const muf_char *);
    manager->modules = mufCreateDict(_MufModule);
    return manager;
}

static void _mufDestroyModuleManager(_MufModuleManager *manager) {
    mufDestroyDict(manager->modules);
    mufFree(manager);
}

static void _mufModuleManagerRegister(_MufModuleManager *manager, const MufModuleRegistry *registry) {
    const muf_char *name = registry->name;
    
    if (mufDictContains(_manager->modules, name)) {
        return;
    }
    
    _MufModule mod;
    mufCStrCopyN(mod.name, name, MUF_MODULE_NAME_MAX_LEN);
    mufCStrCopyN(mod.description, registry->description, MUF_MODULE_DESCRIPTION_MAX_LEN);
    mod.version = registry->version;
    mod.priority = registry->priority;
    mod.status = MUF_MODULE_STATUS_READY;
    mod.loadCallback = registry->loadCallback;
    mod.unloadCallback = registry->unloadCallback;
    mod.loadTime = 0;

    _MufModule *target = (_MufModule *) mufDictInsert(_manager->modules, name, &mod);
    const muf_char *nameBuffer = target->name;
    mufArrayPush(_manager->names, &nameBuffer);
}

static MUF_INLINE _MufModule *_mufModuleManagerGet(_MufModuleManager *manager, const muf_char *modName) {
    return (_MufModule *) mufDictGetRef(manager->modules, modName);
}

static MUF_INLINE const muf_char **_mufModuleManagerGetAllNames(_MufModuleManager *manager, muf_usize *count) {
    *count = mufArrayGetSize(manager->names);
    return (const muf_char **) mufArrayGetData(manager->names);
}

static MUF_INLINE void _mufModuleManagerLoad(_MufModuleManager *manager, _MufModule *mod) {
    if (mod->status == MUF_MODULE_STATUS_LOADED) {
        mufWarn("The module '%s' had been loaded", mod->name);
        return;
    }

    mod->loadTime = (muf_u64) time(NULL);
    if (mod->loadCallback) {
        mod->loadCallback();
    }
    mod->status = MUF_MODULE_STATUS_LOADED;
}

void _mufModuleManagerUnload(_MufModuleManager *manager, _MufModule *mod) {
    if (mod->unloadCallback) {
        mod->unloadCallback();
    }
    mod->status = MUF_MODULE_STATUS_READY;
}

static MUF_INLINE _MufModuleManager *_mufGetModuleManager() {
    if (_manager == NULL) {
        _manager = _mufCreateModuleManager();
    }
    return _manager;
}

void mufRegisterModule(const MufModuleRegistry *registry) {
    _mufModuleManagerRegister(_mufGetModuleManager(), registry);
}

MufModule mufGetModule(const muf_char *modName) {
    _MufModule *mod = (_MufModule *) mufDictGetRef(_mufGetModuleManager()->modules, modName);

    if (!mod) {
        //mufError("Could not find module named with %s", modName);
        return mufNullHandle(MufModule);
    }
    return mufMakeHandle(MufModule, ptr, mod);
}

muf_usize mufGetModuleNames(const muf_char **names) {
    return 0;
}

#define _MOD(_handle) ((_MufModule *) _handle._ptr)

void mufLoadModule(MufModule mod) {
    _mufModuleManagerLoad(_mufGetModuleManager(), _MOD(mod));
}

void mufUnloadModule(MufModule mod) {
    _mufModuleManagerUnload(_mufGetModuleManager(), _MOD(mod));
}

void mufReloadModule(MufModule mod, muf_bool force) {
    mufUnloadModule(mod);
    mufLoadModule(mod);
}

const muf_char *mufModuleGetName(MufModule mod) {
    return _MOD(mod)->name;
}

const muf_char *mufModuleGetDescription(MufModule mod) {
    return _MOD(mod)->description;
}

MufVersion mufModuleGetVersion(MufModule mod) {
    return _MOD(mod)->version;
}

muf_u32 mufModuleGetPriority(MufModule mod) {
    return _MOD(mod)->priority;
}

MufModuleStatus mufModuleGetStatus(MufModule mod) {
    return _MOD(mod)->status;
}

void mufModuleSetPriority(MufModule mod, muf_u32 priority) {
    _MOD(mod)->priority = priority;
}

void mufModuleReportError(MufModule mod, const muf_char *format, ...) {
    _MufModule *m = _MOD(mod);
    muf_char messageBuffer[512];
    va_list args;
    va_start(args, format);
    vsprintf(messageBuffer, format, args);
    va_end(args);
    mufError("<%s> %s", m->name, messageBuffer);
    m->status = MUF_MODULE_STATUS_ERROR;
}

#undef _MOD