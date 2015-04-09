#ifndef __SVC_H__
#define __SVC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void svcSleepThread(s64 nanoseconds);
void svcFlushProcessDataCache(u32 process, void* addr, u32 size);
void svcBackdoor(void (*func)());

#ifdef __cplusplus
};
#endif

#endif