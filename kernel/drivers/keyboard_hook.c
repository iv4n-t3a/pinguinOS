#include "kernel/drivers/keyboard_hook.h"

#include "kernel/shell/shell.h"

void keyboard_hook(keycode_t keycode) { kernel_shell_process_char(keycode); }
