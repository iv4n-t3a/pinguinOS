.code16
.global _start

.macro LOAD msg
    xor %ax, %ax
    mov %ax, %ds
    mov \msg, %si
.endm

_start:
    cli
    LOAD $copying_lower_message
    call println

setup_segment_registers:
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %sp

copy_mbr_lower:
    mov $0x0100, %cx
    mov $0x7C00, %si
    mov $0x0600, %di
    rep movsw
    ljmp $0, $(0x0600 + low_start - _start)

low_start:
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es
    cmp $0x0, %ax
    jne error

    sti
    mov $low_start_success_message, %si
    call println
    call unimplemented

error:
    LOAD $error_message
    call println
error_hang:
    jmp error_hang

unimplemented:
    LOAD $unimplemented_message
    call println
unimplemented_hang:
    jmp unimplemented_hang

.macro PCHAR char
    mov \char, %al
    int $0x10
.endm

println:
    mov $0x0E, %ah
    mov $0, %bh

    // Print 'MBR: ' preffix
    PCHAR $'M'
    PCHAR $'B'
    PCHAR $'R'
    PCHAR $':'
    PCHAR $' '

println_loop:
    lodsb
    test %al, %al
    je println_done
    int $0x10
    jmp println_loop
println_done:
    PCHAR $'\r'
    PCHAR $'\n'
    ret

copying_lower_message:
    .asciz "Copying to lower address..."

low_start_success_message:
    .asciz "Started at low_start."

error_message:
    .asciz "Fatal error."

unimplemented_message:
    .asciz "Further boot unimplemented."

.org 510
.word 0xAA55
