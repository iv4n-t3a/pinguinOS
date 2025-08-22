#ifndef BOOT_STAGE2_LOAD_SECTORS_H
#define BOOT_STAGE2_LOAD_SECTORS_H

#include <stdint.h>

typedef enum {
  LOAD_SECTORS_SUCCESS = 0,
  LOAD_SECTORS_DAPACK_INIT_ERROR = 1,
  LOAD_SECTORS_BIOS_CALL_ERROR = 2
} load_sectors_error_t;

load_sectors_error_t load_sectors(int drive, void *buffer, uint64_t lba,
                                  uint16_t sectors);

#endif // #ifndef BOOT_STAGE2_LOAD_SECTORS_H
