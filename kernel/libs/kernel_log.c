#include "kernel/libs/kernel_log.h"

#include <stdarg.h>

#include "core_lib/stdio.h"
#include "kernel/libs/stdio.h"

#define DEFINE_LOG_FUNC(level)                                                 \
  void log_##level(const char *file, const char *func, int line,               \
                   const char *fmt, ...) {                                     \
    dprintf("[%s] %s:%s:%d ", #level, file, func, line);                       \
    va_list args;                                                              \
    va_start(args, fmt);                                                       \
    generic_printf(dpchar, fmt, args);                                         \
    va_end(args);                                                              \
  }                                                                            \
                                                                               \
  void log_##level##_append(const char *fmt, ...) {                            \
    va_list args;                                                              \
    va_start(args, fmt);                                                       \
    generic_printf(dpchar, fmt, args);                                         \
    va_end(args);                                                              \
  }

// TODO: preopper level separation
#define DEFINE_DISABLED_LOG_FUNC(level)                                        \
  void log_##level(const char *file, const char *func, int line,               \
                   const char *fmt, ...) {}                                    \
  void log_##level##_append(const char *fmt, ...) {}

DEFINE_LOG_FUNC(DEBUG);
DEFINE_LOG_FUNC(INFO);
DEFINE_LOG_FUNC(WARNING);
DEFINE_LOG_FUNC(ERROR);
DEFINE_LOG_FUNC(FATAL);
