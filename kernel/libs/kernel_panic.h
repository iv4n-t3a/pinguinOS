#ifndef KERNEL_LIBS_KERNEL_PANIC_H
#define KERNEL_LIBS_KERNEL_PANIC_H

#include "kernel/libs/kernel_log.h"
#include "kernel/libs/stdio.h"

static inline void kernel_panic() {
  kprintf("KERNEL PANIC!!!\n");
  log_fatal("KERNEL PANIC!!!\n");

  for (;;) {
  }
}

#endif // #ifndef KERNEL_LIBS_KERNEL_LOG
