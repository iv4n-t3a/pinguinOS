#include "kernel/arch/x86/isr.h"

#include "kernel/libs/stdio.h"

DEFINE_ISR_INTERRUPT_GATE_IRQ(ps2_keyboard_handler) {
  char scan_code = port_in_8(0x60);
  kprintf("%c", scan_code);
}
