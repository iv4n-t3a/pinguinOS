#include <stddef.h>

#include "boot/stage2/bios_calls.h"
#include "boot/stage2/load_sectors.h"
#include "core_lib/string.h"

#define MEDIATOR_BUFFER_SIZE 512

const int k_direct_load_max_address = 0xF'FFFF;
const int k_direct_load_max_sectors = 128;

uint8_t mediator_buffer[MEDIATOR_BUFFER_SIZE];

load_sectors_error_t load_sectors_direct(int drive, void *buffer, uint64_t lba,
                                         uint16_t sectors) {
  DAPack dap;

  if (init_DAPack(&dap, buffer, lba, sectors)) {
    return LOAD_SECTORS_DAPACK_INIT_ERROR;
  }

  while (sectors) {
    dap.sectors_read = sectors;
    if (BIOS_read_disk(&dap, drive)) {
      return LOAD_SECTORS_BIOS_CALL_ERROR;
    }
    sectors -= dap.sectors_read;
  }

  return LOAD_SECTORS_SUCCESS;
}

load_sectors_error_t load_sectors_one_by_one(int drive, uint8_t *buffer,
                                             uint64_t lba, uint16_t sectors) {
  for (size_t i = 0; i < sectors; ++i) {
    load_sectors_error_t err =
        load_sectors_direct(drive, mediator_buffer, lba + i, 1);
    if (err) {
      return err;
    }

    memcpy(buffer, mediator_buffer, MEDIATOR_BUFFER_SIZE);
    buffer += MEDIATOR_BUFFER_SIZE;
  }
  return LOAD_SECTORS_SUCCESS;
}

load_sectors_error_t load_sectors(int drive, void *buffer, uint64_t lba,
                                  uint16_t sectors) {
  if ((uint32_t)buffer > k_direct_load_max_address ||
      sectors > k_direct_load_max_address) {
    return load_sectors_one_by_one(drive, buffer, lba, sectors);
  } else {
    return load_sectors_direct(drive, buffer, lba, sectors);
  }
}
