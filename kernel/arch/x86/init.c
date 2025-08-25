#include "core_lib/vga.h"
#include "kernel/arch/x86/gdt.h"
#include "kernel/arch/x86/idt.h"
#include "kernel/arch/x86/idt_handlers.h"

void arch_init() {
  GDT_load();

  interrupt_gate_t default_gate = INTERRUPT_GATE(-1, default_handler);
  IDT_load(nullptr, 0, default_gate);

  terminal_initialize(80, 25);
}
