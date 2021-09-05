#ifndef _MUFFIN_RENDER_INTERNAL_BACKEND_MANAGER_H_
#define _MUFFIN_RENDER_INTERNAL_BACKEND_MANAGER_H_

#include "muffin_core/common.h"
#include "muffin_core/dict.h"
#include "muffin_render/backend.h"

typedef MufGeneratorFunc(MufRenderBackendInitCallback, void);
typedef MufGeneratorFunc(MufRenderBackendFinishCallback, void);

typedef struct _MufRenderBackend_s {
    muf_char                        name[MUF_RENDER_BACKEND_MAX_NAME_LEN];
    MufRenderBackendInitCallback    init;
    MufRenderBackendFinishCallback  finish;
    MufRenderBackendApi             api;
} _MufRenderBackend;

typedef struct _MufRenderBackendManager_s {
    MufDict             *backends;
    _MufRenderBackend   *defaultBackend;
} _MufRenderBackendManager;

_MufRenderBackendManager *_mufCreateRenderBackendManager();
void _mufDestroyRenderBackendManager(_MufRenderBackendManager *manager);
void _mufRenderBackendManagerRegister(_MufRenderBackendManager *manager, const MufRenderBackendRegistry *registry);
_MufRenderBackend *_mufRenderBackendManagerGet(_MufRenderBackendManager *manager, const muf_char *name);
_MufRenderBackend *_mufRenderBackendManagerGetDefault(_MufRenderBackendManager *manager);
void _mufRenderBackendManagerSetDefault(_MufRenderBackendManager *manager, _MufRenderBackend *backend);

#define _MUF_CHECK_BACKEND() MUF_ASSERT(_mufRenderBackendManager->defaultBackend != NULL)
#define _MUF_BACKEND_CHECK_CALL(Func, ...) do { _MUF_CHECK_BACKEND(); _mufRenderBackendManager->defaultBackend->api.Func(__VA_ARGS__); } while (0)
#define _MUF_BACKEND_CALL(Func, ...) _mufRenderBackendManager->defaultBackend->api.Func(__VA_ARGS__)

#endif