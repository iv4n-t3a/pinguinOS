#include "kernel/arch/x86/isr.h"

#include "kernel/drivers/ps2_keyboard.h"
#include "kernel/libs/stdio.h"

DEFINE_ISR_INTERRUPT_GATE_IRQ(ps2_keyboard_handler) {
  char scan_code = port_in_8(k_ps2_keyboard_port);
  ps2_command_hook(scan_code);
}
