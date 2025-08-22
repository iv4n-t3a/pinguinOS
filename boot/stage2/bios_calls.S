.code32

////////////////////////////////////////////////////////////
////////// Mode switch macros

.macro ENTER_REAL_MODE
    ljmpw $3 << 3, $protected_mode_16_start\@

.code16
protected_mode_16_start\@:
    mov %cr0, %eax
    and $~1, %al
    mov %eax, %cr0
    ljmp $0, $real_mode_start\@

real_mode_start\@:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %ss
    sti
.endm

.macro ENTER_PROTECTED_MODE
    mov %cr0, %eax
    or $1, %al
    mov %eax, %cr0
    cli

    ljmp $1 << 3, $protected_mode_start\@

.code32
protected_mode_start\@:
    mov $2 << 3, %ax
    mov %ax, %ds
    mov %ax, %ss
.endm

////////////////////////////////////////////////////////////
////////// Function definitions

.global BIOS_read_disk
BIOS_read_disk:
    push %ebp
    mov %esp, %ebp

    ENTER_REAL_MODE

    mov $0x42, %ah
    mov 12(%bp), %dl
    mov 8(%bp), %si
    int $0x13

    jc BIOS_read_disk_error

BIOS_read_disk_succeed:
    xor %ax, %ax
    jmp BIOS_read_disk_return

BIOS_read_disk_error:
    mov $1, %ax

BIOS_read_disk_return:
    push %ax

    ENTER_PROTECTED_MODE

    xor %eax, %eax
    pop %ax
    mov %ebp, %esp
    pop %ebp
    ret
