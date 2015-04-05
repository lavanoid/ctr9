#include "ctr9/fs.h"

// TODO: Port to multiple FIRM versions.
#define FILE_OPEN 0x0805CF05
#define FILE_CLOSE 0x0805CFC5
#define FILE_READ 0x0804E315
#define FILE_WRITE 0x0805E181
#define FILE_GET_SIZE 0x0805DEF5

u32 __attribute__((naked)) fileOpen(void* handle, const u16* name, u32 mode) {
    asm volatile("push {r4, lr} \t\n"
            "ldr r4, =%0   \t\n"
            "blx r4        \t\n"
            "pop {r4, pc}  \t\n" : : "i"(FILE_OPEN));
}

void __attribute__((naked)) fileClose(void* handle) {
    asm volatile("push {r4, lr} \t\n"
            "ldr r4, =%0   \t\n"
            "blx r4        \t\n"
            "pop {r4, pc}  \t\n" : : "i"(FILE_CLOSE));
}

void __attribute__((naked)) fileRead(void* handle, u32* bytesRead, void* buf, u32 size) {
    asm volatile("push {r4, lr} \t\n"
            "ldr r4, =%0   \t\n"
            "blx r4        \t\n"
            "pop {r4, pc}  \t\n" : : "i"(FILE_READ));
}

void __attribute__((naked)) fileWrite(void* handle, u32* bytesWritten, void* buf, u32 size) {
    asm volatile("push {r4, lr}     \t\n"
            "sub sp, sp, #4    \t\n"
            "ldr r4, [sp, #12] \t\n"
            "str r4, [sp, #0]  \t\n"
            "ldr r4, =%0       \t\n"
            "blx r4            \t\n"
            "add sp, sp, #4    \t\n"
            "pop {r4, pc}      \t\n" : : "i"(FILE_WRITE));
}

void __attribute__((naked)) fileGetSize(void* handle, u32* size) {
    asm volatile("push {r4, lr} \t\n"
            "ldr r4, =%0   \t\n"
            "blx r4        \t\n"
            "pop {r4, pc}  \t\n" : : "i"(FILE_GET_SIZE));
}