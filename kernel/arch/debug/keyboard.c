#include "kernel/arch/debug/keyboard.h"

#include "kernel/drivers/keyboard_hook.h"

#include <pthread.h>
#include <stdio.h>

void *keyboard_reader(void *args) {
  for (;;) {
    char c = getchar();
    keycode_t keycode = {.pressed_char = c};
    keyboard_hook(keycode);
  }
}

void run_keyboard_emulator() {
  pthread_t thread;
  pthread_create(&thread, NULL, keyboard_reader, NULL);
}
