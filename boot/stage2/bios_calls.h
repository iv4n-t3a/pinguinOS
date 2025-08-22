#ifndef BOOT_STAGE2_BIOS_CALLS_H
#define BOOT_STAGE2_BIOS_CALLS_H

#include <stdint.h>

typedef struct {
  uint8_t size;
  uint8_t zero_byte;
  uint16_t sectors_read;
  uint16_t buffer_offset;
  uint16_t buffer_segment;
  uint32_t lba_low;
  uint32_t lba_high;
} __attribute__((packed)) DAPack;

static inline int init_DAPack(DAPack *dap, void *buffer, uint64_t lba,
                              uint16_t sectors) {
  if ((uint32_t)buffer >= 0xF'FFFF) {
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
  dap->lba_low = lba & 0xFFFF'FFFF;
  dap->lba_high = (lba >> 32) & 0xFFFF'FFFF;
  return 0;
}

int __attribute__((cdecl)) BIOS_read_disk(DAPack *, int drive_number);

#endif // #ifndef BOOT_STAGE2_BIOS_CALLS_H
