.code32

////////////////////////////////////////////////////////////
////////// Mode switch macros

.macro ENTER_REAL_MODE
    push %eax

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

    pop %eax
.endm

.macro ENTER_PROTECTED_MODE
    push %eax

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

    pop %eax
.endm

.macro INIT_STACK_FRAME
    push %ebp
    mov %esp, %ebp
.endm

.macro RETURN
    mov %ebp, %esp
    pop %ebp
    ret
.endm

.macro CARRY_TO_EAX
    mov $0, %eax
    adc $0, %eax
.endm

// Assume offset is either di or si and segment is one of segment registers
.macro LINEAR_TO_SEGMENT_OFFSET linear segment offset
    push %eax
    push %ebx

    mov \linear, %eax
    cmp $0xFFFF, %eax

    jle linear_to_segment_offset_use_zero_segment\@
    jmp linear_to_segment_offset_not_use_max_offset\@

linear_to_segment_offset_use_zero_segment\@:

    mov \linear, \offset
    mov $0, %ax
    mov %ax, \segment

    jmp linear_to_segment_offset_return\@

linear_to_segment_offset_not_use_max_offset\@:
    xor %eax, %eax

    mov \linear, %ax
    and $0xF, \linear
    add $0xFFF0, %ax
    mov %ax, \segment

    mov \linear, %ebx
    sub %eax, %ebx
    shr $4, %ebx
    mov %ebx, \segment

linear_to_segment_offset_return\@:

    pop %ebx
    pop %eax
.endm

.macro DEREFERENCE pointer destination
    push %di
    push %ds

    LINEAR_TO_SEGMENT_OFFSET \pointer, %ds, %di
    mov (%di), \destination

    pop %ds
    pop %di
.endm

.macro ASSIGN pointer value
    push %di
    push %ds

    LINEAR_TO_SEGMENT_OFFSET \pointer, %ds, %di
    mov \value, (%di)

    pop %ds
    pop %di
.endm

.macro BIOS_CALL_BEGIN
    INIT_STACK_FRAME
    ENTER_REAL_MODE
.endm

.macro BIOS_CALL_END
    CARRY_TO_EAX
    ENTER_PROTECTED_MODE
    RETURN
.endm

.macro BOCHS_BREAKPOINT
   pushw %ax
   pushw %dx
   movw $0x8A00, %ax
   movw $0x8A00, %dx
   outw %ax, %dx
   movw $0x08AE0, %ax
   outw %ax, %dx
   popw %dx
   popw %ax
.endm

////////////////////////////////////////////////////////////
////////// Function definitions

/*
 * int CDECL BIOS_read_disk(DAPack *, int drive_number);
 */
.global BIOS_read_disk
BIOS_read_disk:
    BIOS_CALL_BEGIN

    // Setup bios call
    mov $0x42, %ah                              /* Call number */
    mov 12(%bp), %dl                            /* drive_number */
    LINEAR_TO_SEGMENT_OFFSET 8(%bp), %ds, %si   /* DApack* */

    // Perform call
    int $0x13

    BIOS_CALL_END

/*
 * int CDECL BIOS_detect_memory_E820(E820_mementry_t *, int *call_id);
 */
.global BIOS_detect_memory_E820
BIOS_detect_memory_E820:
    BIOS_CALL_BEGIN

    // Setup bios call
    mov $0xE820, %eax                          /* Call number */
    DEREFERENCE 12(%bp), %ebx                  /* *call_id */
    mov $24, %ecx                              /* sizeof(E820_mementry_t) */
    mov $0x534D4150, %edx                      /* Magic signature */
    LINEAR_TO_SEGMENT_OFFSET 8(%bp), %es, %di  /* E820_mementry_t address */

    // Perform call
    int $0x15

    pushf
    ASSIGN 12(%ebp), %ebx /* Save call id for next call */
    popf

    BIOS_CALL_END
