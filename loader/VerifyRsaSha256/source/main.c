typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void __attribute__((naked)) svcBackdoor(void (*func)()) {
    asm volatile("svc 0x7B \t\n"
                 "bx lr    \t\n");
}

u32 __attribute__((naked)) fileOpen(void* handle, const u16* name, u32 mode) {
    asm volatile("push {r4, lr}       \t\n"
                 "ldr r4, =0x0805CF05 \t\n"
                 "blx r4              \t\n"
                 "pop {r4, pc}        \t\n");
}

void __attribute__((naked)) fileClose(void* handle) {
    asm volatile("push {r4, lr}       \t\n"
                 "ldr r4, =0x0805CFC5 \t\n"
                 "blx r4              \t\n"
                 "pop {r4, pc}        \t\n");
}

void __attribute__((naked)) fileRead(void* handle, u32* bytesRead, void* buf, u32 size) {
    asm volatile("push {r4, lr}       \t\n"
                 "ldr r4, =0x0804E315 \t\n"
                 "blx r4              \t\n"
                 "pop {r4, pc}        \t\n");
}

void __attribute__((naked)) fileGetSize(void* handle, u32* size) {
    asm volatile("push {r4, lr}       \t\n"
                 "ldr r4, =0x0805DEF5 \t\n"
                 "blx r4              \t\n"
                 "pop {r4, pc}        \t\n");
}

#define ARM11_EXCVEC_ADDR 0x1FFF4000
#define ARM9_PAYLOAD 0x20000000

void killARM11() {
    // Disable IRQ, FIQ, MPU.
    asm volatile("mrs r0, cpsr              \t\n"
                 "orr r0, r0, #0xC0         \t\n"
                 "msr cpsr_c, r0            \t\n"
                 "mrc p15, 0, r0, c1, c0, 0 \t\n"
                 "bic r0, r0, #1            \t\n"
                 "mcr p15, 0, r0, c1, c0, 0 \t\n");

    for(u32 i = 0; i < 8; i++) {
        *(volatile u32*) (ARM11_EXCVEC_ADDR + i * 4) = 0xEAFFFFFE;
    }

    // Enable IRQ, FIQ, MPU.
    asm volatile("mrs r0, cpsr              \t\n"
                 "bic r0, r0, #0xC0         \t\n"
                 "msr cpsr_c, r0            \t\n"
                 "mrc p15, 0, r0, c1, c0, 0 \t\n"
                 "orr r0, r0, #1            \t\n"
                 "mcr p15, 0, r0, c1, c0, 0 \t\n");
}

void executeARM9() {
    // Disable IRQ, FIQ, MPU.
    asm volatile("mrs r0, cpsr              \t\n"
                 "orr r0, r0, #0xC0         \t\n"
                 "msr cpsr_c, r0            \t\n"
                 "mrc p15, 0, r0, c1, c0, 0 \t\n"
                 "bic r0, r0, #1            \t\n"
                 "mcr p15, 0, r0, c1, c0, 0 \t\n");

    ((void(*)()) ARM9_PAYLOAD)();
}

int main() {
    svcBackdoor(&killARM11);

    u8 handle[32];
    for(u32 i = 0; i < sizeof(handle); i++) {
        handle[i] = 0;
    }

    u32 result = fileOpen(&handle, u"sdmc:/arm9.bin", 0x1);
    if(result != 0) {
        return 0;
    }

    u32 fileSize;
    fileGetSize(&handle, &fileSize);

    u32 bytesRead;
    fileRead(&handle, &bytesRead, (void*) ARM9_PAYLOAD, fileSize);
    if(bytesRead != fileSize) {
        return 0;
    }

    fileClose(&handle);

    svcBackdoor(&executeARM9);
    return 0;
}