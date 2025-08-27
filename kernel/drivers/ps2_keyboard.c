#include "kernel/drivers/ps2_keyboard.h"

#include "kernel/drivers/keyboard.h"
#include "kernel/libs/stdio.h"
#include "kernel/shell/shell.h"

static const char scan_code_table[] = "??1234567890-=\b"
                                      "?qwertyuiop[]"
                                      "\n?asdfgh"
                                      "jkl;\'`?\\zxcv"
                                      "bnm,./??? ";

void ps2_command_hook(uint8_t scan_code) {
  if (scan_code > sizeof(scan_code_table)) {
    return;
  }
  char c = scan_code_table[scan_code];
  keycode_t keycode = {.pressed_char = c};
  // TODO: multiple tty, device files and propper drivers organization
  kernel_shell_process_char(keycode);
}
