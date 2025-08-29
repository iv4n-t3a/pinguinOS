#include "core_lib/boot_params.h"
#include "core_lib/vga.h"
#include "kernel/arch/x86/config.h"
#include "kernel/arch/x86/gdt.h"
#include "kernel/arch/x86/idt.h"
#include "kernel/arch/x86/isr.h"
#include "kernel/arch/x86/pic.h"

void arch_init(boot_params_t params) {
  terminal_initialize(k_terminal_width, k_terminal_height);

  GDT_load();

  PIC_init(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
  PIC_mask_irq(k_pic_mask);

  IDT_load(idt_handlers, sizeof(idt_handlers) / sizeof(idtentry_t));
}
