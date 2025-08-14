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
////////// BPB (BIOS Parameter Block)

_start:
    jmp main
    nop

    .ascii "iv4n-t3a"     /* OEM identifier */
    .word  512            /* Bytes per sector */
    .byte  4              /* Sectors per cluster */
    .word  4              /* Reserved sectors */
    .byte  2              /* FAT count */
    .word  512            /* Root directory entries */
    .word  0              /* 16-bit total sectors for volume */
    .byte  0xF8           /* Media descriptor type */
    .word  256            /* Number of sectors per FAT */
    .word  63             /* Sectors per track for int 0x13 */
    .word  16             /* Number of heads for int 0x13 */
    .int   1              /* LBA of the beginning of the partition */
    .int   0x40000        /* 128Mb. 32-bit total sectors per volume. */
// FAT16 Specific fields
    .byte  0x80           /* Drive number */
    .byte  0              /* Flags */
    .byte  0x29           /* Signature */
    .int   0              /* Volume serial number */
    .ascii "NO NAME    "  /* Volume label string */
    .ascii "FAT16   "     /* FAT type string representation */

////////////////////////////////////////////////////////////
////////// Executable code

main:
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

    // Print 'VBR: ' preffix
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
