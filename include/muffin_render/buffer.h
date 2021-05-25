#ifndef _MUFFIN_RENDER_BUFFER_H_
#define _MUFFIN_RENDER_BUFFER_H_

#include "muffin_core/common.h"
#include "muffin_render/format.h"

typedef enum MufBufferType_e {
    MUF_BUFFER_TYPE_VERTEX,
    MUF_BUFFER_TYPE_INDEX,
    MUF_BUFFER_TYPE_UNIFORM,
    MUF_BUFFER_TYPE_INDIRECT
} MufBufferType;

typedef enum MufBufferUsage_e {
    MUF_BUFFER_USAGE_STATIC,
    MUF_BUFFER_USAGE_DYNAMIC,
    MUF_BUFFER_USAGE_STREAM
} MufBufferUsage;

typedef enum MufBufferCPUAccessFlags_e {
    MUF_BUFFER_CPU_ACCESS_FLAGS_READ = 0x01,
    MUF_BUFFER_CPU_ACCESS_FLAGS_WRITE = 0x02,
    MUF_BUFFER_CPU_ACCESS_FLAGS_READ_WRITE = 0x03
} MufBufferCPUAccessFlags;

typedef struct MufBufferCreateInfo_s {
    MufBufferType       type;
    muf_usize           alignment;
    muf_usize           size;
    const muf_rawptr    data;
    MufBufferUsage      usage;
} MufBufferCreateInfo;

typedef MufHandle MufBuffer;

MUF_API MufBuffer mufCreateBuffer(const MufBufferCreateInfo *info);
MUF_API void mufDestroyBuffer(MufBuffer buffer);
MUF_API void mufBufferUpdate(MufBuffer buffer, muf_usize offset, const muf_rawptr data, muf_usize size);
MUF_API void mufBufferClear(MufBuffer buffer, MufFormat internalForamt, MufFormat format, const muf_rawptr *data);
MUF_API muf_usize mufBufferGetSize(MufBuffer buffer);
MUF_API MufBufferUsage mufBufferGetUsage(MufBuffer buffer);

#endif