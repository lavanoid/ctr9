.section ".init"
.global _start
.extern main
.align 4
.arm

_start:
    bl main

.pool

loop:
    ldr r0, =0xFFFFFFFF
    svc 0x0A
    b loop
