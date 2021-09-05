#ifndef _MUFFIN_PLATFORM_THREAD_H_
#define _MUFFIN_PLATFORM_THREAD_H_

#include "muffin_core/common.h"

typedef struct MufThread_s { } MufThread;

typedef muf_rawptr(*MufThreadCallback)(muf_rawptr data);

MufThread *mufCreateThread(const muf_char *name, MufThreadCallback callback);
void mufThreadDestroy(MufThread *thread);
void mufThreadJoin(MufThread *thread);

const muf_char *mufThreadGetName(MufThread thread);

typedef struct MufMutex_s { } MufMutex;

MufMutex *mufCreateMutex();
void mufDestroyMutex(MufMutex *mutex);

void mufMutexLock(MufMutex *mutex);
void mufMutexUnlock(MufMutex *mutex);

#endif