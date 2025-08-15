.code16
.global _start

////////////////////////////////////////////////////////////
////////// BPB (BIOS Parameter Block)

_start:
    jmp main
    nop

BPB_OEM_identifier:        .ascii "iv4n-t3a"
BPB_bytes_per_sector:      .word  512
BPB_sectors_per_cluster:   .byte  4
BPB_reserved_sectors:      .word  4
BPB_FAT_count:             .byte  2
BPB_root_dir_entries:      .word  512
BPB_16_sectors_per_volume: .word  0
BPB_media_descriptor_type: .byte  0xF8
BPB_sectors_per_fat:       .word  256
BPB_sectors_per_track:     .word  63 /* For int 0x13 */
BPB_number_of_heads:       .word  16 /* For int 0x13 */
BPB_part_begin_lba:        .int   1
BPB_32_sectors_per_volume: .int   0x40000

// FAT16 Specific
BPB_drive_number:          .byte  0x80
BPB_flags:                 .byte  0
BPB_signature:             .byte  0x29
BPB_volume_serial_number:  .int   0
BPB_volume_label_string:   .ascii "NO NAME    "
BPB_FAT_type_as_string:    .ascii "FAT16   "

////////////////////////////////////////////////////////////
////////// Memory layout

// General memory layout
.set root_dir_address,   0x0500
.set vbr_address,        0x7C00
.set DAP_address,        0x7E00
.set variables_address,  0x7F00
.set kernel_address,     0x8000

// Dapack
.set DAP_packet_size,              0x7E00
.set DAP_zero,                     0x7E01
.set DAP_read_sectors,             0x7E02
.set DAP_transfer_buffer_offset,   0x7E04
.set DAP_transfer_buffer_segment,  0x7E06
.set DAP_starting_LBA_lower,       0x7E08
.set DAP_starting_LBA_upper,       0x7E0C

// Variables
.set fat_start_LBA,               0x7F10
.set root_dir_start_LBA,          0x7F12
.set root_dir_size,               0x7F14
.set root_dir_not_loaded_sectors, 0x7F16
.set root_dir_writting_address,   0x7F18
.set root_dir_reading_LBA,        0x7F1A

////////////////////////////////////////////////////////////
////////// Executable code

main:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $0x7FFFF, %sp
    sti

// BPB_reserved_sectors + BPB_part_begin_lba
calc_fat_start_LBA:
    mov (BPB_part_begin_lba), %ax
    add (BPB_reserved_sectors), %ax
    mov %ax, (fat_start_LBA)

// fat_start_LBA + BPB_FAT_count * BPB_sectors_per_fat
calc_root_dir_start_LBA:
    mov (fat_start_LBA), %ax
    mov %ax, (root_dir_start_LBA)

    mov (BPB_FAT_count), %ax
    mov (BPB_sectors_per_fat), %cx
    mul %cx
    add %ax, (root_dir_start_LBA)

// (BPB_root_dir_entries * 32 + BPB_bytes_per_sector - 1) / BPB_bytes_per_sector
// or
// round_cell(BPB_root_dir_entries * 32 / BPB_bytes_per_sector)
calc_root_dir_size:
    mov (BPB_root_dir_entries), %ax
    mov $32, %cx
    mul %cx

    add (BPB_bytes_per_sector), %ax
    sub $1, %ax

    mov (BPB_bytes_per_sector), %cx
    div %cx
    mov %ax, (root_dir_size)

load_root_directory:
    mov (root_dir_size), %ax
    mov %ax, (root_dir_not_loaded_sectors)
    mov $root_dir_address, (root_dir_writting_address)
    mov (root_dir_start_LBA), %ax
    mov %ax, (root_dir_reading_LBA)

load_root_directory_loop:
    mov  (root_dir_not_loaded_sectors), %ax
    mov  (root_dir_writting_address),   %bx
    mov  (root_dir_reading_LBA),        %cx

    movb $0x10, (DAP_packet_size)
    movb $0,    (DAP_zero)
    mov  %ax,   (DAP_read_sectors)
    mov  $0,    (DAP_transfer_buffer_segment)
    mov  %bx,   (DAP_transfer_buffer_offset)
    mov  %cx,   (DAP_starting_LBA_lower)
    mov  $0,    (DAP_starting_LBA_upper)

    mov $0x42, %ah
    mov (BPB_drive_number), %dl
    mov $DAP_address, %si

    int $0x13

    mov %ah, (DAP_zero)
    jc error
    mov (DAP_read_sectors), %ax  /* sectors actualy read */

    sub %ax, (root_dir_not_loaded_sectors)
    jz load_root_directory_loop_finish

    mov (DAP_read_sectors), %ax
    add %ax, (root_dir_reading_LBA)

    mov (BPB_bytes_per_sector), %bx
    mul %bx
    add %ax, (root_dir_writting_address)

    jmp load_root_directory_loop

load_root_directory_loop_finish:

find_kernel_file:
    mov $root_dir_address, %bx
    mov (BPB_root_dir_entries), %cx

find_kernel_loop:
    push %cx
    mov $kernel_filename, %si
    mov %bx,              %di
    mov $11, %cx
    repe cmpsb

    je find_kernel_loop_finish

    add $32, %bx
    pop %cx
    dec %cx
    jz error

    jmp find_kernel_loop

find_kernel_loop_finish:
    jmp unimplemented

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
    mov $0x0E, %ah
    mov $0, %bh

    PCHAR $'E'
    PCHAR $'R'
    PCHAR $'R'
    jmp .

unimplemented:
    mov $0x0E, %ah
    mov $0, %bh

    PCHAR $'U'
    jmp .

////////////////////////////////////////////////////////////
////////// Constants

kernel_filename:
  .asciz "KERNEL  BIN"
