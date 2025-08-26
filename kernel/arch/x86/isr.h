#ifndef KERNEL_ARCH_X86_ISR_H
#define KERNEL_ARCH_X86_ISR_H

#include "kernel/arch/x86/config.h"
#include "kernel/arch/x86/idt.h"

DECLARE_ISR_INTERRUPT_GATE_IRQ(ps2_keyboard_handler);

static const idtentry_t idt_handlers[] = {
    INTERRUPT_GATE_IDTENTRY_IRQ(PIC_MASTER_OFFSET + 1, ps2_keyboard_handler),
};

#endif // #ifndef KERNEL_ARCH_X86_ISR_H
