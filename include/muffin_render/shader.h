#ifndef _MUFFIN_RENDER_SHADER_H_
#define _MUFFIN_RENDER_SHADER_H_

#include "muffin_core/common.h"

typedef enum MufShaderType_e {
    MUF_SHADER_TYPE_VERTEX      = 0,
    MUF_SHADER_TYPE_FRAGMENT    = 1,
    MUF_SHADER_TYPE_COMPUTE     = 2
} MufShaderType;

typedef enum MufShaderSourceType_e {
    MUF_SHADER_SOURCE_TYPE_TEXT     = 0,
    MUF_SHADER_SOURCE_TYPE_BINARY   = 1,
    MUF_SHADER_SOURCE_TYPE_EXT      = 2
} MufShaderSourceType;

typedef struct MufShaderCreateInfo_s {
    MufShaderType       stageType;
    MufShaderSourceType sourceType;
    const muf_rawptr    source;
    muf_usize           sourceSize;
} MufShaderCreateInfo;

typedef struct MufShaderProgramCreateInfo_s {
    muf_usize shaderCount;
    MufShader *shaders;
    muf_bool  autoDestroy;
} MufShaderProgramCreateInfo;

typedef MufHandle MufShader;
typedef MufHandle MufShaderProgram;

MUF_API MufShader mufCreateShader(const MufShaderCreateInfo *info);
MUF_API void mufDestroyShader(MufShader shader);
MUF_API MufShaderProgram mufCreateShaderProgram(const MufShaderProgramCreateInfo *info);
MUF_API void mufDestroyShaderProgram(MufShaderProgram program);
MUF_API void mufShaderProgramGetErrorReport(MufShaderProgram shader);

#endif