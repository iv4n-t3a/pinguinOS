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
////////// MBR code

_start:
    cli

save_boot_drive:
    mov %dl, boot_drive
    call print_boot_drive

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

check_partitions:
    mov $PT1, %bx
    mov $4, %cx

check_partitions_loop:
    mov (%bx), %al
    test $0x80, %al
    jz check_partitions_failure
    call check_partitions_success

check_partitions_failure:
    add $0x10, %bx
    dec %cx
    jnz check_partitions_loop
    call error

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
    cmpw $0xAA55, (0x7DFE)
    jne error

    mov selected_partition, %si
    mov boot_drive, %di
    ljmp $0, $0x7C00
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

boot_drive_message:
    .asciz "Booting from drive "

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
  .byte 0x00  /* Ending sector 63, cylinder high bits 3 */
  .byte 0x00  /* Ending cylinder low bits */
  .long 1     /* Starting LBA (sector 2048) */
  .long 1     /* Number of sectors (approx. 100MB) */
PT2:
  .fill 16, 1, 0
PT3:
  .fill 16, 1, 0
PT4:
  .fill 16, 1, 0
