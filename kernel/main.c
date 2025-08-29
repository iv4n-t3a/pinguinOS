#include "core_lib/boot_params.h"
#include "kernel/config.h"
#include "kernel/libs/kernel_log.h"
#include "kernel/shell/shell.h"

// kernel/arch/.../init.c
void arch_init(boot_params_t);

void log_boot_params_x86(x86_boot_params_t params) {
  log_debug("boot_params.x86_boot_params.memory_regions_count = %d\n",
            params.memory_regions_count);
  log_debug("boot_params.x86_boot_params.memory_regions = [\n");

  for (size_t i = 0; i < params.memory_regions_count; ++i) {
    x86_mementry_t *region = params.memory_regions + i;
    log_debug("  Memory region: base=%x, length=%x, type=",
              (uint32_t)region->base, (uint32_t)region->length);

    switch (region->type) {
    case RAM_TYPE_USABLE:
      log_debug("usable");
      break;
    case RAM_TYPE_RESERVED:
      log_debug("reserved");
      break;
    case RAM_TYPE_ACPI_RECLAIMABLE:
      log_debug("acpi reclaimable");
      break;
    case RAM_TYPE_ACPI_NVS:
      log_debug("acpi nvs");
      break;
    case RAM_TYPE_BAD_MEMORY:
      log_debug("bad memory");
      break;
    }

    log_debug(", ACPI=%d\n", region->ACPI);
  }
  log_debug("]\n");
}

void log_boot_params(boot_params_t params) {
  log_debug("boot_params.boot_drive = %x\n", params.boot_drive);
  log_debug("boot_params.stack_begin = %x\n", params.stack_begin);

#if ARCH_X86
  log_boot_params_x86(params.x86_boot_params);
#endif
}

void kmain(boot_params_t params) {
  arch_init(params);

  log_debug("Pinguin OS kernel initialized\n");
  log_boot_params(params);

  log_debug("Initializing shell...\n");
  kernel_shell_init();
  log_debug("Shell initialized\n");

  for (;;) {
  }
}
