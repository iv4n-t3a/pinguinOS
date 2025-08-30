#include "kernel/arch/x86/isr.h"

#include "kernel/drivers/ps2_keyboard.h"
#include "kernel/libs/kernel_log.h"

DEFINE_ISR_INTERRUPT_GATE_IRQ(ps2_keyboard_handler) {
  LOG_DEBUG("ps2 keyboard handler isr hooked\n");
  char scan_code = port_in_8(k_ps2_keyboard_port);
  ps2_command_hook(scan_code);
}
