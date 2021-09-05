#include "muffin_render/render.mod.h"

#include "internal/backend_manager.h"

extern _MufRenderBackendManager *_mufRenderBackendManager;

static void _load() {
    _mufRenderBackendManager = _mufCreateRenderBackendManager();
}

static void _unload() {
    _mufDestroyRenderBackendManager(_mufRenderBackendManager);
}

MUF_MODULE_REGISTRY_DEF(Render,
    "MF_render",
    "",
    mufCreateVersion(1, 0, 0),
    0,
    _load,
    _unload
);