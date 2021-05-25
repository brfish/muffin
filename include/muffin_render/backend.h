#ifndef _MUFFIN_RENDER_BACKEND_H_
#define _MUFFIN_RENDER_BACKEND_H_

#include "muffin_core/common.h"

struct MufShaderProgramCreateInfo;
struct MufBufferCreateInfo;
struct MufPipelineCreateInfo;

typedef struct MufRenderBackend_s {
    muf_char name[32];

    MufHandle (* createBuffer)(const MufBufferCreateInfo *info);
    MufHandle (* createMapBuffer);

    MufHandle (* createFramebuffer)();

    void (* destroyBuffer)(MufHandle handle);
    MufHandle (* createShader)(const MufShaderProgramCreateInfo *info);

    MufHandle (* createPipeline)(const MufPipelineCreateInfo *info);
} MufRenderBackend;

MufRenderBackend mufLoadRenderBackend(const muf_char *name);

#endif