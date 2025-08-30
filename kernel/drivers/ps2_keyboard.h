#ifndef KERNEL_DRIVERS_PS2_KEYBOARD_H
#define KERNEL_DRIVERS_PS2_KEYBOARD_H

#include <stdint.h>

// Seek definition in kernel/arch/.../ps2_keyboard.c
void ps2_byte_out(char c);

// Seek definition in kernel/arch/.../ps2_keyboard.c
char ps2_byte_in();

void ps2_keyboard_init();

void ps2_command_hook(uint8_t scan_code);

#endif // #ifndef KERNEL_DRIVERS_PS2_KEYBOARD_H
