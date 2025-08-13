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
////////// VBR code

_start:
    PRINTLN $vbr_successful_start_message
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
    jmp .

unimplemented:
    PRINTLN $unimplemented_message
    jmp .

print:
    mov $0x0E, %ah
    mov $0, %bh

    // Print 'MBR: ' preffix
    PCHAR $'V'
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

////////////////////////////////////////////////////////////
////////// Messages

vbr_successful_start_message:
    .asciz "VBR started successfuly"

error_message:
    .asciz "Fatal error."

unimplemented_message:
    .asciz "Further boot unimplemented."
