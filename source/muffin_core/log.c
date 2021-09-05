#include "muffin_core/log.h"

#include <stdarg.h>
#include <time.h>

#include "muffin_core/array.h"

static const muf_char *_MUF_LOG_LEVEL_STRS[] = {
    "Track",
    "Debug",
    "Info",
    "Warn",
    "Error",
    "Fatal"
};

static MufLogger *_mufCreateBasicLogger(MufLoggerType type) {
    MufLogger *logger = mufAlloc(MufLogger, 1);
    logger->type = type;
    logger->level = MUF_LOG_LEVEL_TRACK;
    for (muf_index i = 0; i < _MUF_LOG_LEVEL_COUNT_; ++i) {
        logger->enabled[i] = MUF_TRUE;
    }
    return logger;
}

MufLogger *mufCreateConsoleLogger(muf_bool useStderr) {
    return _mufCreateBasicLogger(useStderr ? MUF_LOGGER_TYPE_CONSOLE_STDERR : 
        MUF_LOGGER_TYPE_CONSOLE_STDOUT);
}

MufLogger *mufCreateFileLogger(const muf_char *filePath) {
    MufLogger *logger =_mufCreateBasicLogger(MUF_LOGGER_TYPE_FILE);
    logger->fileStream = fopen(filePath, "a");
    return logger;
}

void mufDestroyLogger(MufLogger *logger) {
    if (logger->type == MUF_LOGGER_TYPE_FILE) {
        fclose(logger->fileStream);
    }
    mufFree(logger);
}

MufLoggerType mufLoggerGetType(const MufLogger *logger) {
    return logger->type;
}

MufLogLevel mufLoggerGetLevel(const MufLogger *logger) {
    return logger->level;
}

muf_bool mufLoggerIsEnabled(const MufLogger *logger, MufLogLevel level) {
    return logger->enabled[(muf_index) level];
}

void mufLoggerSetLevel(MufLogger *logger, MufLogLevel level) {
    logger->level = level;
}

void mufLoggerEnable(MufLogger *logger, MufLogLevel level) {
    logger->enabled[(muf_index) level] = MUF_TRUE;
}

void mufLoggerDisable(MufLogger *logger, MufLogLevel level)  {
    logger->enabled[(muf_index) level] = MUF_FALSE;
}

static void _logFormat(FILE *file, MufLogLevel level, const muf_char *format, va_list args) {
    time_t systemTime;
    time(&systemTime);
    struct tm *localTime = localtime(&systemTime);
    fprintf(file, "%d-%02d-%02d %02d:%02d:%02d ", 1900 + localTime->tm_year, localTime->tm_mon, localTime->tm_mday,
        localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    fprintf(file, "[%s] ", _MUF_LOG_LEVEL_STRS[(muf_index) level]);
    vfprintf(file, format, args);
    fprintf(file, "\n");
    fflush(file);
}

static void _logFormatAssertion(FILE *file, const muf_char *filename, muf_index line, 
    const muf_char *expression, const muf_char *format, va_list args) {
    time_t systemTime;
    time(&systemTime);
    struct tm *localTime = localtime(&systemTime);
    fprintf(file, "%d-%02d-%02d %02d:%02d:%02d ", 1900 + localTime->tm_year, localTime->tm_mon, localTime->tm_mday,
        localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    fprintf(file, "[Assertion] ");
    fprintf(file, "%s:%" PRIindex "\n\tExpected: %s\n\tMessage: ", filename, line, expression);
    vfprintf(file, format, args);
    fprintf(file, "\n");
    fflush(file);
}

void mufLoggerLog(MufLogger *logger, MufLogLevel level, const muf_char *format, ...) {
    if (!logger->enabled[(muf_index) level]) {
        return;
    }
    va_list args;
    va_start(args, format);
    switch (logger->type) {
        case MUF_LOGGER_TYPE_CONSOLE_STDOUT: _logFormat(stdout, level, format, args); break;
        case MUF_LOGGER_TYPE_CONSOLE_STDERR: _logFormat(stderr, level, format, args); break;
        case MUF_LOGGER_TYPE_FILE: _logFormat(logger->fileStream, level, format, args); break;
    }
    va_end(args);
}

static void _mufLoggerVLog(MufLogger *logger, MufLogLevel level, const muf_char *format, va_list args) {
    if (!logger->enabled[(muf_index) level]) {
        return;
    }
    switch (logger->type) {
        case MUF_LOGGER_TYPE_CONSOLE_STDOUT: _logFormat(stdout, level, format, args); break;
        case MUF_LOGGER_TYPE_CONSOLE_STDERR: _logFormat(stderr, level, format, args); break;
        case MUF_LOGGER_TYPE_FILE: _logFormat(logger->fileStream, level, format, args); break;
    }
}

typedef struct _MufGlobalLogger_s {
    muf_bool    initialized;
    MufLogger   *console;
    MufLogger   *file;
    muf_bool    consoleEnabled;
    muf_bool    fileEnabled;
} _MufGlobalLogger;

_MufGlobalLogger _mufGlobalLogger[1] = { { 0 } };

static MUF_INLINE _MufGlobalLogger *_getGlobalLogger(void) {
    if (!_mufGlobalLogger->initialized) {
        _mufGlobalLogger->console = mufCreateConsoleLogger(MUF_FALSE);
        _mufGlobalLogger->file = mufCreateFileLogger("logs/muffin.log");
        _mufGlobalLogger->consoleEnabled = MUF_FALSE;
        _mufGlobalLogger->fileEnabled = MUF_TRUE;
        _mufGlobalLogger->initialized = MUF_TRUE;
    }
    return _mufGlobalLogger;
}

void mufLog(MufLogLevel level, const muf_char *format, ...) {
    _MufGlobalLogger *g = _getGlobalLogger();
    va_list args;
    va_start(args, format);
    if (g->fileEnabled) {
        _mufLoggerVLog(g->file, level, format, args);
    }
    if (g->consoleEnabled) {
        _mufLoggerVLog(g->console, level, format, args);
    }
    va_end(args);
}

void _mufAssertLogFormatPrint(const muf_char *filename, muf_index line,
    const muf_char *expression, const muf_char *format, ...) {
    _MufGlobalLogger *g = _getGlobalLogger();

    va_list args;
    va_start(args, format);
    if (g->consoleEnabled) {
        _logFormatAssertion(stdout, filename, line, expression, format, args);
    }
    if (g->fileEnabled) {
        _logFormatAssertion(g->file->fileStream, filename, line, expression, format, args);
    }
    va_end(args);
}

void mufSetLogLevel(MufLogLevel level) {
    _MufGlobalLogger *g = _getGlobalLogger();
    g->file->level = level;
    g->console->level = level;
}

void mufEnableConsoleLog(void) {
    _getGlobalLogger()->consoleEnabled = MUF_TRUE;
}

void mufDisableConsoleLog(void) {
    _getGlobalLogger()->consoleEnabled = MUF_FALSE;
}

void mufEnableLogFile(void) {
    _getGlobalLogger()->fileEnabled = MUF_TRUE;
}

void mufDisbaleLogFile(void) {
    _getGlobalLogger()->fileEnabled = MUF_FALSE;
}