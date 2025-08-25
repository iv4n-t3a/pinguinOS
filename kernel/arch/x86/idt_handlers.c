#include "kernel/arch/x86/idt_handlers.h"

#include "kernel/libs/kernel_log.h"

DEFINE_INTERRUPT(default_handler) {
  log_debug("Unknown interrupt.\n"
            "Registers: \n"
            "  eax: %d\n"
            "  ebx: %d\n"
            "  ecx: %d\n"
            "  edx: %d\n"
            "  ebp: %d\n"
            "  esp: %d\n"
            "  esi: %d\n"
            "  edi: %d\n"
            "  eip: %d\n"
            "  cs: %d\n"
            "  ss: %d\n"
            "  ds: %d\n"
            "  eflags: %d\n",
            regs->eax, regs->ebx, regs->ecx, regs->edx, regs->ebp, regs->esp,
            regs->esi, regs->edi, regs->eip, regs->cs, regs->ss, regs->ds,
            regs->eflags);
}
