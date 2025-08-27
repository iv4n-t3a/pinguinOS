#include "kernel/drivers/ps2_keyboard.h"

#include "kernel/arch/x86/config.h"
#include "kernel/arch/x86/io.h"

void ps2_byte_out(char c) { port_out_8(k_ps2_keyboard_port, c); }

char ps2_byte_in() { return port_in_8(k_ps2_keyboard_port); }
