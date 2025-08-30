#ifndef KERNEL_LIBS_KERNEL_PANIC_H
#define KERNEL_LIBS_KERNEL_PANIC_H

#include "kernel/libs/kernel_log.h"
#include "kernel/libs/stdio.h"

#define KERNEL_PANIC(fmt, ...)                                                 \
  LOG_FATAL(fmt, ##__VA_ARGS__);                                               \
  LOG_FATAL("KERNEL PANIC!!!\n");                                              \
  kernel_panic(__FILE__, __func__, __LINE__)

static inline void kernel_panic(const char *file, const char *func, int line) {
  kprintf("%s:%s:%d KERNEL PANIC!!!\n", file, func, line);

  for (;;) {
  }
}

#endif // #ifndef KERNEL_LIBS_KERNEL_LOG
