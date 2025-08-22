#include "kernel_libs/vga.h"

void kmain() {
  terminal_initialize(80, 25);
  terminal_writestring("Hello, World!\n");

  for (;;) {
  }
}
