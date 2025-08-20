#include "stdbool.h"
#include "libs/vga.h"
#include <stdint.h>

void bootmain(void) {
  terminal_initialize(80, 25);
  terminal_writestring("Hello, stage 2 bootloader World!\n");

  for(;;) {}
}
