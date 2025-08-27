#ifndef KERNEL_SHELL_SHELL_H
#define KERNEL_SHELL_SHELL_H

#include "kernel/drivers/keyboard.h"

// TODO: Propper shell in user-space

void kernel_shell_init();

void kernel_shell_process_char(keycode_t key);

#endif // #ifndef KERNEL_SHELL_SHELL_H
