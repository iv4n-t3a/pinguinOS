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
BPB_32_sectors_per_volume: .int   0x10000

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
.set stack_begin,        0x7C00
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

// Partition info
.set fat_start_LBA,               0x7F10
.set root_dir_start_LBA,          0x7F12
.set root_dir_size,               0x7F14
.set data_region_begin_LBA,       0x7F16
// Load sectors from disk related
.set disk_reading_sectors_left,   0x7F18
.set disk_reading_dst,            0x7F1A
.set disk_reading_LBA,            0x7F1C
// Kernel load related
.set kload_current_cluster,       0x7F20
.set kload_write_iter,            0x7F22

////////////////////////////////////////////////////////////
////////// Executable code

main:
    cli
    xor %ax, %ax
    mov %ax, %cs
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $stack_begin, %sp
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

// root_dir_start_LBA + root_dir_size
calc_data_region_begin_LBA:
    mov (root_dir_start_LBA), %ax
    mov (root_dir_size), %bx
    add %bx, %ax
    mov %ax, (data_region_begin_LBA)

load_root_dir:
    mov (root_dir_size), %ax
    mov %ax, (disk_reading_sectors_left)
    mov $root_dir_address, (disk_reading_dst)
    mov (root_dir_start_LBA), %ax
    mov %ax, (disk_reading_LBA)
    mov $0,  (disk_reading_LBA + 2)
    call load_segment_from_disk

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

load_kernel:
    add $26, %bx
    mov (%bx), %bx
    mov %bx, (kload_current_cluster)

    mov $kernel_address, (kload_write_iter)

load_kernel_loop:
    call info
    // lba = data_region_begin + (kload_current_cluster - 2) * BPB_sectors_per_cluster
    mov (kload_current_cluster), %ax
    sub $2, %ax
    xor %dx, %dx
    xor %bx, %bx
    movb (BPB_sectors_per_cluster), %bl
    mul %bx
    add (data_region_begin_LBA), %ax
    mov %dx, (disk_reading_LBA+2)
    mov %ax, (disk_reading_LBA)

    mov (kload_write_iter), %ax
    mov %ax, (disk_reading_dst)

    xor %ax, %ax
    movb (BPB_sectors_per_cluster), %al
    mov %ax, (disk_reading_sectors_left)

    call load_segment_from_disk

    mov (kload_write_iter), %ax
    mov (BPB_sectors_per_cluster), %bx
    add %bx, %ax
    mov %ax, (kload_write_iter)

    mov (kload_current_cluster), %ax
    shl $1, %ax
    mov $root_dir_address, %bx
    add %ax, %bx
    mov (%bx), %ax

    cmp $0xFFF8, %ax
    je unimplemented
    cmp $0xFFFF, %ax
    je unimplemented
    cmp $0xF8FF, %ax
    je unimplemented
    jmp error

    cmp $0xFFF8, %ax
    jge load_kernel_finish
    mov %ax, (kload_current_cluster)
    jmp load_kernel_loop

load_kernel_finish:
    ljmp $0, $kernel_address

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
    PCHAR $'O'
    PCHAR $'R'
    jmp .

unimplemented:
    mov $0x0E, %ah
    mov $0, %bh

    PCHAR $'U'
    jmp .

info:
    mov $0x0E, %ah
    mov $0, %bh

    PCHAR $'I'
    ret

load_segment_from_disk:

load_segment_from_disk_loop:
    mov  (disk_reading_sectors_left), %ax
    mov  (disk_reading_dst),          %bx

    movb $0x10, (DAP_packet_size)
    movb $0,    (DAP_zero)
    mov  %ax,   (DAP_read_sectors)
    mov  $0,    (DAP_transfer_buffer_segment)
    mov  %bx,   (DAP_transfer_buffer_offset)

    mov  (disk_reading_LBA),          %ax
    mov  (disk_reading_LBA + 2),      %bx

    mov  %ax,   (DAP_starting_LBA_lower)
    mov  %bx,   (DAP_starting_LBA_lower + 2)
    mov  $0,    (DAP_starting_LBA_upper)
    mov  $0,    (DAP_starting_LBA_upper + 2)

    mov $0x42, %ah
    mov (BPB_drive_number), %dl
    mov $DAP_address, %si

    int $0x13

    mov %ah, (DAP_zero)
    jc error
    mov (DAP_read_sectors), %ax  /* sectors actualy read */


load_segment_from_disk_loop_finish:
    ret

////////////////////////////////////////////////////////////
////////// Constants

kernel_filename:
  .asciz "KERNEL  BIN"
