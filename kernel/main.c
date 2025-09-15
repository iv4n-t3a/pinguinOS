#include "core_lib/boot_params.h"
#include "kernel/config.h"
#include "kernel/libs/kernel_log.h"
#include "kernel/log_boot_params.h"
#include "kernel/shell/shell.h"

// kernel/arch/.../init.c
void arch_init(boot_params_t);

void kmain(boot_params_t params) {
  arch_init(params);

  LOG_INFO("Pinguin OS kernel initialized\n");
  log_boot_params(params);

  LOG_INFO("Initializing shell...\n");
  kernel_shell_init();
  LOG_INFO("Shell initialized\n");

  for (;;) {
  }
}
