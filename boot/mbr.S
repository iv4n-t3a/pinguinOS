.code16
.global _start

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

.macro SET_DATA_SEGMENT seg
    cli
    mov \seg, %ax
    mov %ax, %ds
    mov %ax, %es
    sti
.endm

////////////////////////////////////////////////////////////
////////// MBR code

_start:

    // Update code segment
    ljmp $0x7C0, $skip
skip:

setup_segment_registers:
    cli
    xor %ax, %ax
    mov %ax, %ss
    mov $stack_begin, %sp
    sti

    SET_DATA_SEGMENT $high_segment

save_boot_drive:
    mov %dl, boot_drive
    call print_boot_drive

    // Empty boot signature for further checks
    mov $0, (0x1FE)

copy_mbr_lower:
    SET_DATA_SEGMENT $0

    mov $0x0100, %cx
    mov $0x7C00, %si
    mov $0x0600, %di
    rep movsw
    ljmp $0, $(0x0600 + low_start - _start)

low_start:
    SET_DATA_SEGMENT $low_segment

check_partitions:
    mov $PT1, %bx
    mov $4, %cx

check_partitions_loop:
    mov (%bx), %al
    test $0x80, %al
    jz check_partitions_failure
    jmp check_partitions_success

check_partitions_failure:
    add $0x10, %bx
    dec %cx
    jnz check_partitions_loop
    jmp error

check_partitions_success:
    mov %bx, selected_partition

read_vbr:
    mov 8(%bx), %ax
    mov %ax, selected_partition_dapack_lower_LBA
    mov 10(%bx), %ax
    mov %ax, selected_partition_dapack_lower_LBA+2

    // Setup BIOS call
    mov $selected_partition_dapack, %si
    mov $0x42, %ah
    mov $boot_drive, %bx
    mov (%bx), %dl
    int $0x13
    jc error

jump_to_vbr:
    mov selected_partition, %si
    mov boot_drive, %di

    SET_DATA_SEGMENT $high_segment
    cmpw $0xAA55, (0x1FE)
    jne no_boot_signature

    ljmp $0, $0x7C00

no_boot_signature:
    SET_DATA_SEGMENT $low_segment
    PRINTLN $no_boot_signature_message
    jmp .

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

error_message:
    .asciz "Fatal error."

unimplemented_message:
    .asciz "Further boot unimplemented."

no_boot_signature_message:
    .asciz "Boot signature check failed"

boot_drive_message:
    .asciz "Booting from drive "

////////////////////////////////////////////////////////////
////////// Constatns

.equ stack_begin, 0xFFF0
.equ low_segment, 0x60
.equ high_segment, 0x7C0

////////////////////////////////////////////////////////////
////////// Variables

boot_drive:
    .int 0

selected_partition:
    .int 0

selected_partition_dapack:
    .byte  0x10        /* Size of packet */
    .byte  0
    .short 1           /* Number of sectors to transfer */
    .int   0x7C00      /* Transfer buffer */
selected_partition_dapack_lower_LBA:
    .int   0
selected_partition_dapack_upper_LBA:
    .int   0

////////////////////////////////////////////////////////////
////////// Partition table

.section .partition_table

UID:
  .fill 6, 1, 0
PT1:
  // Active testing partition
  .byte 0x80  /* Boot indicator: Active */
  .byte 0x00  /* Starting head */
  .byte 0x01  /* Starting sector 1, cylinder high bits 0 */
  .byte 0x00  /* Starting cylinder low bits */
  .byte 0x83  /* Partition type: Linux */
  .byte 0x01  /* Ending head */
  .byte 0x00  /* Ending sector */
  .byte 0x00  /* Ending cylinder low bits */
  .long 1     /* Starting LBA */
  .long 1     /* Number of sectors (approx. 100MB) */
PT2:
  .fill 16, 1, 0
PT3:
  .fill 16, 1, 0
PT4:
  .fill 16, 1, 0
