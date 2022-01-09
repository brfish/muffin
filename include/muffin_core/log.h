#ifndef _MUFFIN_CORE_LOG_H_
#define _MUFFIN_CORE_LOG_H_

#include "muffin_core/array.h"
#include "muffin_core/common.h"

typedef enum MufLogLevel_e {
    MUF_LOG_LEVEL_TRACK = 0U,
    MUF_LOG_LEVEL_DEBUG,
    MUF_LOG_LEVEL_INFO,
    MUF_LOG_LEVEL_WARN,
    MUF_LOG_LEVEL_ERROR,
    MUF_LOG_LEVEL_FATAL,
    MUF_ENUM_COUNT(MUF_LOG_LEVEL)
} MufLogLevel;

typedef enum MufLoggerType_e {
    MUF_LOGGER_TYPE_CONSOLE_STDOUT,
    MUF_LOGGER_TYPE_CONSOLE_STDERR,
    MUF_LOGGER_TYPE_FILE
} MufLoggerType;

typedef struct MufLogger_s {
    MufLoggerType   type;
    MufLogLevel     level;
    FILE            *fileStream;
    muf_bool        enabled[_MUF_LOG_LEVEL_COUNT_];
} MufLogger;

MufLogger *mufCreateConsoleLogger(muf_bool useStderr);
MufLogger *mufCreateFileLogger(const muf_char *filePath);

MUF_API void mufDestroyLogger(MufLogger *logger);

MUF_API MufLoggerType mufLoggerGetType(const MufLogger *logger);
MUF_API MufLogLevel mufLoggerGetLevel(const MufLogger *logger);
MUF_API muf_bool mufLoggerIsEnabled(const MufLogger *logger, MufLogLevel level);

MUF_API void mufLoggerSetLevel(MufLogger *logger, MufLogLevel level);
MUF_API void mufLoggerEnable(MufLogger *logger, MufLogLevel level);
MUF_API void mufLoggerDisable(MufLogger *logger, MufLogLevel level);

MUF_API void mufLoggerLog(MufLogger *logger, MufLogLevel level, const muf_char *format, ...);
#define mufLoggerTrack(_logger, _format, ...) mufLoggerLog(_logger, MUF_LOG_LEVEL_TRACK, _format, __VA_ARGS__)
#define mufLoggerDebug(_logger, _format, ...) mufLoggerLog(_logger, MUF_LOG_LEVEL_DEBUG, _format, __VA_ARGS__)
#define mufLoggerInfo(_logger, _format, ...)  mufLoggerLog(_logger, MUF_LOG_LEVEL_INFO , _format, __VA_ARGS__)
#define mufLoggerWarn(_logger, _format, ...)  mufLoggerLog(_logger, MUF_LOG_LEVEL_WARN , _format, __VA_ARGS__)
#define mufLoggerError(_logger, _format, ...) mufLoggerLog(_logger, MUF_LOG_LEVEL_ERROR, _format, __VA_ARGS__)
#define mufLoggerFatal(_logger, _format, ...) mufLoggerLog(_logger, MUF_LOG_LEVEL_FATAL, _format, __VA_ARGS__)

MUF_API void mufLog(MufLogLevel level, const muf_char *format, ...);
#define mufTrack(...) mufLog(MUF_LOG_LEVEL_TRACK, __VA_ARGS__)
#define mufDebug(...) mufLog(MUF_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define mufInfo(...)  mufLog(MUF_LOG_LEVEL_INFO , __VA_ARGS__)
#define mufWarn(...)  mufLog(MUF_LOG_LEVEL_WARN , __VA_ARGS__)
#define mufError(...) mufLog(MUF_LOG_LEVEL_ERROR, __VA_ARGS__)
#define mufFatal(...) mufLog(MUF_LOG_LEVEL_FATAL, __VA_ARGS__)

void _mufAssertLogFormatPrint(const muf_char *filename, muf_index line,
    const muf_char *expression, const muf_char *format, ...);

#if defined(MUF_DEBUG)
#   define MUF_ASSERT_LOG(_expr) \
        do { if (!(_expr)) { _mufAssertLogFormatPrint(__FILE__, __LINE__, #_expr, "Assertion failed"); MUF_DEBUGBREAK(); } } while (0)
#   define MUF_FASSERT_LOG(_expr, Fmt, ...) \
        do { if (!(_expr)) { _mufAssertLogFormatPrint(__FILE__, __LINE__, #_expr, Fmt, ##__VA_ARGS__); MUF_DEBUGBREAK(); } } while (0)
#else
#   define MUF_ASSERT_LOG(_expr)
#   define MUF_FASSERT_LOG(_expr, Fmt, ...)
#endif

void mufSetLogLevel(MufLogLevel level);
void mufEnableConsoleLog(void);
void mufDisableConsoleLog(void);
void mufEnableLogFile(void);
void mufDisbaleLogFile(void);

#endif