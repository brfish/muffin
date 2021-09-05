#include "muffin_render/pipeline.h"

#include "internal/backend_manager.h"

extern _MufRenderBackendManager *_mufRenderBackendManager;

MufPipeline mufCreatePipeline(const MufPipelineCreateInfo *info) {
    _MUF_CHECK_BACKEND();
    return _MUF_BACKEND_CALL(createPipeline, info);}

void mufDestroyPipeline(MufPipeline pipeline) {
    _MUF_BACKEND_CHECK_CALL(destroyPipeline, pipeline);
}