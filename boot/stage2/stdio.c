#include "boot/stage2/stdio.h"

#include <stdarg.h>

#include "core_lib/stdio.h"
#include "core_lib/vga.h"

void putstr(const char *str) { generic_putstr(terminal_putchar, str); }

void printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(terminal_putchar, fmt, args);
  va_end(args);
}
