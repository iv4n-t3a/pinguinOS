.code16
.global _start

.macro LOAD msg
    xor %ax, %ax
    mov %ax, %ds
    mov \msg, %si
.endm

.start:
    cli
    LOAD $message
    call print

hang:
    jmp hang

print:
    lodsb
    test %al, %al
    je done
    mov $0x0E, %ah
    mov $0, %bh
    int $0x10
    jmp print
done:
    ret

message:
    .asciz "Hello, World!"

.org 510
.word 0xAA55
