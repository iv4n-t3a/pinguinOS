#ifndef CORE_LIB_BOOT_PARAMS_H
#define CORE_LIB_BOOT_PARAMS_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
  RAM_TYPE_USABLE,
  RAM_TYPE_RESERVED,
  RAM_TYPE_ACPI_RECLAIMABLE,
  RAM_TYPE_ACPI_NVS,
  RAM_TYPE_BAD_MEMORY
} x86_ram_type_t;

typedef struct {
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t ACPI;
} x86_mementry_t;

typedef struct {
  x86_mementry_t memory_regions[128];
  size_t memory_regions_count;
} x86_boot_params_t;

typedef struct {
  int boot_drive;
  void *stack_begin;

  x86_boot_params_t x86_boot_params;
} boot_params_t;

#endif // #ifndef CORE_LIB_BOOT_PARAMS_H
