#ifndef KERNEL_DRIVERS_KEYBOARD_H
#define KERNEL_DRIVERS_KEYBOARD_H

typedef struct {
  char pressed_char;

  // TODO: Flags. Shift, caps lock, locale e.g.
} keycode_t;

#endif // #ifndef KERNEL_DRIVERS_KEYBOARD_H
