#include "boot/stage2/detect_memory.h"

#include <stddef.h>

#include "boot/stage2/bios_calls.h"
#include "boot/stage2/error.h"

error_t detect_memory_E820(x86_mementry_t *entries, size_t buffer_size,
                           size_t *regions_found) {
  int call_id = 0;
  E820_mementry_t entry;

  for (size_t i = *regions_found; i < buffer_size; ++i) {
    entry.ACPI = 1;

    if (BIOS_detect_memory_E820(&entry, &call_id)) {
      return NEW_ERR(ERR_BIOS_CALL_ERROR);
    }

    entries[i].base = entry.base;
    entries[i].length = entry.length;
    entries[i].type = entry.type;
    entries[i].ACPI = entry.ACPI;

    if (call_id == 0) {
      *regions_found = i + 1;
      return NEW_ERR(ERR_SUCCESS);
    }
  }

  return NEW_ERR(ERR_BUFFER_OVERFLOW);
}

error_t detect_memory(x86_mementry_t *entries, size_t buffer_size,
                      size_t *regions_found) {
  ERR_HANDLE_SUBROUTINE(
      detect_memory_E820(entries, buffer_size, regions_found));
}
