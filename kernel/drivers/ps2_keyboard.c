#include "kernel/drivers/ps2_keyboard.h"

#include <stdbool.h>
#include <stdint.h>

#include "kernel/drivers/keyboard.h"
#include "kernel/drivers/keyboard_hook.h"
#include "kernel/libs/kernel_log.h"

static const char scan_code_table[] = "??1234567890-=\b"
                                      "?qwertyuiop[]"
                                      "\n?asdfghjkl;\'`"
                                      "?\\zxcvbnm,./??? ";

static const char scan_code_table_shift[] = "??!@#$%^&*()_+\b"
                                            "?QWERTYUIOP{}"
                                            "\n?ASDFGHJKL:\"~"
                                            "?|ZXCVBNM<>???? ";

typedef enum {
  KEY_LEFT_SHIFT_PRESSED = 0x2A,
  KEY_RIGHT_SHIFT_PRESSED = 0x36,
  KEY_LEFT_SHIFT_RELEASED = 0xAA,
  KEY_RIGHT_SHIFT_RELEASED = 0xB6,
  KEY_CAPS_LOCK_PRESSED = 0x3A,
  KEY_CAPS_LOCK_RELEASED = 0xBA,
} special_key_t;

typedef struct {
  bool shift;
  bool caps_lock;
} keyboard_state_t;

keyboard_state_t state;

void ps2_keyboard_init() {
  state.shift = false;
  state.caps_lock = false;
}

void ps2_command_hook(uint8_t scan_code) {
  switch (scan_code) {
  case KEY_CAPS_LOCK_PRESSED:
    state.caps_lock = !state.caps_lock;
    break;
  case KEY_LEFT_SHIFT_PRESSED:
  case KEY_RIGHT_SHIFT_PRESSED:
    state.shift = true;
    break;
  case KEY_LEFT_SHIFT_RELEASED:
  case KEY_RIGHT_SHIFT_RELEASED:
    state.shift = false;
    break;
  default:
    if (scan_code > sizeof(scan_code_table)) {
      return;
    }

    char c = state.shift ^ state.caps_lock ? scan_code_table_shift[scan_code]
                                           : scan_code_table[scan_code];
    keycode_t keycode = {.pressed_char = c};
    // TODO: multiple tty, device files and propper drivers organization
    keyboard_hook(keycode);
    break;
  }

  LOG_DEBUG("Ps2 command hook run. state.shift=%d, state.caps_lock=%d\n",
            state.shift, state.caps_lock);
}
