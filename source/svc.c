#include "ctr9/svc.h"

void __attribute__((naked)) svcSleepThread(s64 nanoseconds) {
    asm volatile("svc 0x0A \t\n"
                 "bx lr    \t\n");
}

void __attribute__((naked)) svcFlushProcessDataCache(u32 process, void* addr, u32 size) {
    asm volatile("svc 0x54 \t\n"
                 "bx lr    \t\n");
}

void __attribute__((naked)) svcBackdoor(void (*func)()) {
    asm volatile("svc 0x7B \t\n"
                 "bx lr    \t\n");
}