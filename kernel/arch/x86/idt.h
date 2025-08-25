#ifndef KERNEL_ARCH_X86_IDT_H
#define KERNEL_ARCH_X86_IDT_H

#include <stddef.h>
#include <stdint.h>

#include "kernel/compiler.h"

typedef enum {
  GATE_TYPE_TASK = 5,
  GATE_TYPE_INTERUPT16 = 6,
  GATE_TYPE_TRAP16 = 7,
  GATE_TYPE_INTERUPT32 = 14,
  GATE_TYPE_TRAP32 = 15
} gate_type_t;

typedef struct {
  int interrupt;
  void *offset;
  int segment;
  gate_type_t type;
  int dpl;
} interrupt_gate_t;

typedef struct {
  uint32_t ds;

  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t old_esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t esp;
  uint32_t ss;
} PACKED registers_t;

#define DECLARE_INTERRUPT(func)                                                \
  void CDECL func(registers_t *);                                              \
  void asm_##func();

#define DEFINE_INTERRUPT(func)                                                 \
  asm(".global asm_" #func "\n"                                                \
      "asm_" #func ": \n"                                                      \
      "    pusha\n"                                                            \
      "    mov %ds, %eax\n"                                                    \
      "    push %eax\n"                                                        \
      "    mov $2 << 3, %ax\n"                                                 \
      "    mov %ax, %ds\n"                                                     \
      "    push %esp\n"                                                        \
      "    call " #func "\n"                                                   \
      "    pop %eax\n"                                                         \
      "    pop %eax\n"                                                         \
      "    mov %eax, %ds\n"                                                    \
      "    popa\n"                                                             \
      "    iret");                                                             \
                                                                               \
  void func(registers_t *regs)

#define INTERRUPT_GATE(interrupt, func)                                        \
  {interrupt, asm_##func, 1 << 3, GATE_TYPE_INTERUPT32, 0}

void IDT_load(interrupt_gate_t *gates, size_t count,
              interrupt_gate_t defualt_gate);

#endif // #ifndef KERNEL_ARCH_X86_IDT_H
