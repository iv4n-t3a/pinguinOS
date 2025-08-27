#include "kernel/shell/shell.h"
#include "kernel/libs/kernel_log.h"

void arch_init();

void kmain() {
  arch_init();

  kernel_shell_init();

  for (;;) {
  }
}
