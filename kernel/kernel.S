// This is simple 16-bit real world "Hello, world!"
// to check if kernel loaded from fs and started

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
////////// Executable code

main:
    PRINTLN $hello_world
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

    // Print 'KERNEL: ' preffix
    PCHAR $'K'
    PCHAR $'E'
    PCHAR $'R'
    PCHAR $'N'
    PCHAR $'E'
    PCHAR $'L'
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

hello_world:
    .asciz "Hello, World!"

error_message:
    .asciz "Fatal error."

unimplemented_message:
    .asciz "Unimplemented hit."
