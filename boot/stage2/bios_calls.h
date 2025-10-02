#ifndef BOOT_STAGE2_BIOS_CALLS_H
#define BOOT_STAGE2_BIOS_CALLS_H

#include <stdint.h>

#include "boot/stage2/compiler.h"
#include "core_lib/compiler.h"

typedef struct {
  uint8_t size;
  uint8_t zero_byte;
  uint16_t sectors_read;
  uint16_t buffer_offset;
  uint16_t buffer_segment;
  uint32_t lba_low;
  uint32_t lba_high;
} PACKED DAPack;

static inline int init_DAPack(DAPack *dap, void *buffer, uint64_t lba,
                              uint16_t sectors) {
  if ((uint32_t)buffer >= 0xFFFFF) {
    return 1;
  }
  dap->size = 16;
  dap->zero_byte = 0;
  dap->sectors_read = sectors;
  if ((uint32_t)buffer <= 0xFFFF) {
    dap->buffer_offset = (uint16_t)buffer;
    dap->buffer_segment = 0;
  } else {
    dap->buffer_offset = 0xFFF0 + (uint16_t)buffer & 0xF;
    dap->buffer_segment = ((uint32_t)buffer - dap->buffer_offset) >> 4;
  }
  dap->lba_low = lba & 0xFFFFFFFF;
  dap->lba_high = (lba >> 32) & 0xFFFFFFFF;
  return 0;
}

int CDECL BIOS_read_disk(DAPack *, int drive_number);

typedef enum {
  E820_RAM_TYPE_USABLE = 1,
  E820_RAM_TYPE_RESERVED = 2,
  E820_RAM_TYPE_ACPI_RECLAIMABLE = 3,
  E820_RAM_TYPE_ACPI_NVS = 4,
  E820_RAM_TYPE_BAD_MEMORY = 5
} E820_ram_type_t;

typedef struct {
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t ACPI;
} PACKED E820_mementry_t;

int CDECL BIOS_detect_memory_E820(E820_mementry_t *, int *call_id);

#endif // #ifndef BOOT_STAGE2_BIOS_CALLS_H
