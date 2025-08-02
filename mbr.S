.code16
.global _start

////////////////////////////////////////////////////////////
////////// Main code helper macros

.macro PRINTLN msg
    xor %ax, %ax
    mov %ax, %ds
    mov \msg, %si
    call println
.endm

.macro PRINT msg
    xor %ax, %ax
    mov %ax, %ds
    mov \msg, %si
    call print
.endm

////////////////////////////////////////////////////////////
////////// Mbr code

_start:
    cli

save_boot_drive:
    mov %dl, boot_drive
    call print_boot_drive
    PRINTLN $copying_lower_message

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
    PRINTLN $low_start_success_message

check_partitions:
    call unimplemented

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
error_hang:
    jmp error_hang

unimplemented:
    PRINTLN $unimplemented_message
unimplemented_hang:
    jmp unimplemented_hang

print:
    mov $0x0E, %ah
    mov $0, %bh

    // Print 'MBR: ' preffix
    PCHAR $'M'
    PCHAR $'B'
    PCHAR $'R'
    PCHAR $':'
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

print_boot_drive:
    PRINT $boot_drive_message
    PCHAR $'0'
    PCHAR $'x'
    PCHAR $'8'
    mov boot_drive, %al
    sub $0x80, %al
    PDIGIT %al
    jmp println_done

////////////////////////////////////////////////////////////
////////// Messages

copying_lower_message:
    .asciz "Copying to lower address..."

low_start_success_message:
    .asciz "Started at low_start."

error_message:
    .asciz "Fatal error."

unimplemented_message:
    .asciz "Further boot unimplemented."

boot_drive_message:
    .asciz "Booting from drive "

////////////////////////////////////////////////////////////
////////// Variables

boot_drive:
    .int 0

////////////////////////////////////////////////////////////
////////// MBR signature

.org 510
.word 0xAA55
