.code16
.section .entry, "ax"
.global _start

////////////////////////////////////////////////////////////
////////// Memory layout

.equ stack_begin, 0x8000
.equ code_begin,  0x8000

////////////////////////////////////////////////////////////
////////// Main code helper macros

.macro PRINTLN msg
    mov \msg, %si
    call println
.endm

.macro PRINT msg
    mov \msg, %si
    call print
.endm

////////////////////////////////////////////////////////////
////////// Executable code

_start:
    ljmp $0x0, $main
main:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %ss

    mov $stack_begin, %sp
    sti

    mov %dl, (boot_drive)

    PRINTLN $boot1_greating_message

enable_a20:
    in $0x92, %al
    or $2, %al
    out %al, $0x92

load_gdt:
    lgdt GDT_descriptor

enable_protected:
    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0

jump_protected:
    cli
    ljmp $1 << 3, $protected_start

.code32

protected_start:

setup_segment_registers:
    mov $2 << 3, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

setup_stack:
    mov $stack_begin, %ebp
    mov $stack_begin, %esp

    push (boot_drive)

    call bootmain

1:  hlt
    jmp 1b
.code16

////////////////////////////////////////////////////////////
////////// Helper functions helper macros

.macro PCHAR char
    mov \char, %al
    int $0x10
.endm

.macro PDIGIT digit
    mov \digit, %al

    add $6, %al
    xor $0xf, %al
    sub $6, %al

    add $'0', %al
    int $0x10
.endm

////////////////////////////////////////////////////////////
////////// Helper functions

error:
    PRINTLN $error_message
    jmp .

unimplemented:
    PRINTLN $unimplemented_message
    jmp .

print:
    mov $0x0E, %ah
    mov $0, %bh

    // Print 'STAGE2: ' preffix
    PCHAR $'S'
    PCHAR $'T'
    PCHAR $'A'
    PCHAR $'G'
    PCHAR $'E'
    PCHAR $'2'
    PCHAR $' '
print_loop:
    lodsb
    test %al, %al
    je print_done
    int $0x10
    jmp print_loop
print_done:
    ret

println:
    call print
println_done:
    PCHAR $'\r'
    PCHAR $'\n'
    ret

////////////////////////////////////////////////////////////
////////// GDT

.macro GDT_ENTRY base, limit, flags, access_byte
  .word \limit & 0xFFFF
  .word \base & 0xFFFF
  .byte (\base > 16) & 0xFF
  .byte \access_byte
  .byte ((\limit > 16) & 0xF) | (\flags << 4)
  .byte \base > 24
.endm

.equ GDT_FLAGS_GRANULARITY, 8 /* If set limit is 4K block */
.equ GDT_FLAGS_SIZE, 4 /* 32-bit pm descriptor if set. 16-bit pm else */
.equ GDT_FLAGS_LONG_MODE, 1

.equ GDT_AB_PRESENT_BIT, 0x80 /* Allows an entry to refer to a valid segment */
.equ GDT_AB_DPL0, 0x00 /* Descriptor privilege level */
.equ GDT_AB_DPL1, 0x20
.equ GDT_AB_DPL2, 0x40
.equ GDT_AB_DPL3, 0x60
.equ GDT_AB_TYPE_BIT, 0x10 /* Set if code or data segment. System seg else */
.equ GDT_AB_EXECUTABLE, 0x08
.equ GDT_AB_CONFORMING_BIT, 0x04 /* Set if can be executed from an equal or lower dpl */
.equ GDT_AB_DIRECTION_BIT, 0x04 /* Set if grows down for data segment */
.equ GDT_AB_CODE_READABLE, 0x02
.equ GDT_AB_DATA_WRITABLE, 0x02
.equ GDT_AB_ACCESS_BIT, 0x01

.equ GDT_32_PROTECTED_FLAGS, GDT_FLAGS_GRANULARITY | GDT_FLAGS_SIZE

.equ CODE_SEGMENT_AB, GDT_AB_PRESENT_BIT | GDT_AB_TYPE_BIT | GDT_AB_EXECUTABLE | GDT_AB_CODE_READABLE
.equ DATA_SEGMENT_AB, GDT_AB_PRESENT_BIT | GDT_AB_TYPE_BIT | GDT_AB_DATA_WRITABLE

// Alignment
.balign 4

GDT_begin:
    // Null descriptor
    GDT_ENTRY 0, 0, 0, 0
    // 32-bit descriptors
    GDT_ENTRY 0, 0xFFFFFFFF, GDT_32_PROTECTED_FLAGS, CODE_SEGMENT_AB
    GDT_ENTRY 0, 0xFFFFFFFF, GDT_32_PROTECTED_FLAGS, DATA_SEGMENT_AB
    // 16-bit descriptors
    GDT_ENTRY 0, 0xFFFFFFFF, 0, CODE_SEGMENT_AB
    GDT_ENTRY 0, 0xFFFFFFFF, 0, DATA_SEGMENT_AB
GDT_end:

GDT_descriptor:
    .word GDT_end - GDT_begin - 1
    .int GDT_begin

////////////////////////////////////////////////////////////
////////// Variables

boot_drive:
    .int 0

////////////////////////////////////////////////////////////
////////// Messages

boot1_greating_message:
    .asciz "Bootloader second stage started"

error_message:
    .asciz "Fatal error."

unimplemented_message:
    .asciz "Unimplemented hit."
