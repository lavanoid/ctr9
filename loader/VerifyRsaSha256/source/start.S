.section .data
.balign 4
run_once: .word 1

.section .text.start
.arm
.global _start
.type _start, %function
_start:
    ldr r0, =run_once
    mov r1, #0
    ldr r2, [r0]
    cmp r2, r1
    beq suicide
    str r1, [r0]

    bl main

suicide:
    svc 0x09
    b suicide
