#include "backend_manager.h"

#include "muffin_core/memory.h"
#include "muffin_core/string.h"

_MufRenderBackendManager *_mufCreateRenderBackendManager() {
    _MufRenderBackendManager *manager = mufAlloc(_MufRenderBackendManager, 1);
    manager->backends = mufCreateDict(_MufRenderBackend);
    manager->defaultBackend = NULL;
    return manager;
}

static void _finishAllBackends(const muf_char *key, muf_rawptr value) {
    _MufRenderBackend *backend = (_MufRenderBackend *) value;
    backend->finish();
}

void _mufDestroyRenderBackendManager(_MufRenderBackendManager *manager) {
    mufDictForEach(manager->backends, _finishAllBackends);
    mufDestroyDict(manager->backends);
    mufFree(manager);
}

void _mufRenderBackendManagerRegister(_MufRenderBackendManager *manager, const MufRenderBackendRegistry *registry) {
    MufDict *backends = manager->backends;
    if (mufDictContains(backends, registry->name)) {
        return;
    }
    
    _MufRenderBackend backend;
    mufCStrCopyN(backend.name, registry->name, MUF_RENDER_BACKEND_MAX_NAME_LEN);
    backend.init = registry->init;
    backend.finish = registry->finish;
    backend.api = registry->api;
    mufDictInsert(backends, registry->name, &backend);

    backend.init();
}

_MufRenderBackend *_mufRenderBackendManagerGet(_MufRenderBackendManager *manager, const muf_char *name) {
    _MufRenderBackend *backend = (_MufRenderBackend *) mufDictGetRef(manager->backends, name);

    return backend;
}

_MufRenderBackend *_mufRenderBackendManagerGetDefault(_MufRenderBackendManager *manager) {
    return manager->defaultBackend;
}

void _mufRenderBackendManagerSetDefault(_MufRenderBackendManager *manager, _MufRenderBackend *backend) {
    manager->defaultBackend = backend;
}