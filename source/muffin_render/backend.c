#include "muffin_render/backend.h"

#include "internal/backend_manager.h"
#include "muffin_core/dict.h"
#include "muffin_core/log.h"

_MufRenderBackendManager *_mufRenderBackendManager = NULL;

MufRenderBackend mufGetDefaultRenderBackend() {
    return mufMakeHandle(MufRenderBackend, ptr, _mufRenderBackendManagerGetDefault(_mufRenderBackendManager));
}

MufRenderBackend mufGetRenderBackend(const muf_char *name) {
    return mufMakeHandle(MufRenderBackend, ptr, _mufRenderBackendManagerGet(_mufRenderBackendManager, name));
}

void mufRegisterRenderBackend(const MufRenderBackendRegistry *registry) {
    return _mufRenderBackendManagerRegister(_mufRenderBackendManager, registry);
}

void mufSetDefaultRenderBackend(MufRenderBackend backend) {
    _mufRenderBackendManagerSetDefault(_mufRenderBackendManager, backend._ptr);
}

const muf_char *mufRenderBackendGetName(MufRenderBackend backend) {
    return mufHandleCastPtr(_MufRenderBackend, backend)->name;
}

MufRenderBackendApi *mufRenderBackendGetApi(MufRenderBackend backend) {
    return &mufHandleCastPtr(_MufRenderBackend, backend)->api;
}