#include "core_lib/boot_params.h"
#include "kernel/config.h"
#include "kernel/drivers/init.h"
#include "kernel/libs/kernel_log.h"
#include "kernel/shell/shell.h"

// kernel/arch/.../init.c
void arch_init(boot_params_t);

void log_boot_params_x86(x86_boot_params_t params) {
  LOG_INFO_APPEND("boot_params.x86_boot_params.memory_regions_count = %d\n",
                   params.memory_regions_count);
  LOG_INFO_APPEND("boot_params.x86_boot_params.memory_regions = [\n");

  for (size_t i = 0; i < params.memory_regions_count; ++i) {
    x86_mementry_t *region = params.memory_regions + i;
    LOG_INFO_APPEND("  Memory region: base=%x, length=%x, type=",
                     (uint32_t)region->base, (uint32_t)region->length);

    switch (region->type) {
    case RAM_TYPE_USABLE:
      LOG_INFO_APPEND("usable");
      break;
    case RAM_TYPE_RESERVED:
      LOG_INFO_APPEND("reserved");
      break;
    case RAM_TYPE_ACPI_RECLAIMABLE:
      LOG_INFO_APPEND("acpi reclaimable");
      break;
    case RAM_TYPE_ACPI_NVS:
      LOG_INFO_APPEND("acpi nvs");
      break;
    case RAM_TYPE_BAD_MEMORY:
      LOG_INFO_APPEND("bad memory");
      break;
    }

    LOG_INFO_APPEND(", ACPI=%d\n", region->ACPI);
  }
  LOG_INFO_APPEND("]\n");
}

void log_boot_params(boot_params_t params) {
  LOG_INFO("boot_params.boot_drive = %x\n", params.boot_drive);
  LOG_INFO_APPEND("boot_params.stack_begin = %x\n", params.stack_begin);

#if ARCH_X86
  log_boot_params_x86(params.x86_boot_params);
#endif
}

void kmain(boot_params_t params) {
  arch_init(params);
  drivers_init(params);

  LOG_INFO("Pinguin OS kernel initialized\n");
  log_boot_params(params);

  LOG_INFO("Initializing shell...\n");
  kernel_shell_init();
  LOG_INFO("Shell initialized\n");

  for (;;) {
  }
}
