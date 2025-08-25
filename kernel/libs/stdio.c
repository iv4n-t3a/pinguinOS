#include "kernel/libs/stdio.h"

#include <stdarg.h>

#include "core_lib/stdio.h"

void kputstr(const char *str) { generic_putstr(kpchar, str); }

void kprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(kpchar, fmt, args);
  va_end(args);
}
