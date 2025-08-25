#include "kernel/libs/kernel_log.h"

#include <stdarg.h>

#include "core_lib/stdio.h"
#include "kernel/libs/stdio.h"

void log_debug(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(dpchar, fmt, args);
  va_end(args);
}

void log_info(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(dpchar, fmt, args);
  va_end(args);
}

void log_warning(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(dpchar, fmt, args);
  va_end(args);
}

void log_error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(dpchar, fmt, args);
  va_end(args);
}

void log_fatal(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  generic_printf(dpchar, fmt, args);
  va_end(args);
}
