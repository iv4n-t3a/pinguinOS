#ifndef BOOT_STAGE2_LOAD_FILE_H
#define BOOT_STAGE2_LOAD_FILE_H

#include "kernel_libs/mbr.h"
#include "kernel_libs/fat16.h"

typedef enum {
  INIT_FAT_SUCCESS = 0,
  INIT_FAT_DAPACK_INIT_ERROR = 1,
  INIT_FAT_BIOS_CALL_ERROR = 2,
  INIT_FAT_INCOMPATIBLE_FAT_BUFFER_SIZE,
  INIT_FAT_SIGNATURE_VERIFICATION_FAILED
} init_fat_error_t;

typedef enum {
  LOAD_FILE_SUCCESS = 0,
  LOAD_FILE_DAPACK_INIT_ERROR = 1,
  LOAD_FILE_BIOS_CALL_ERROR = 2,
  LOAD_FILE_INVALID_FILE_NAME,
  LOAD_FILE_FILE_NOT_FOUND
} load_file_error_t;

load_file_error_t init_fat(int drive, MBR_partition_entry_t *entry);

load_file_error_t load_file(uint8_t *buffer, char *name);

#endif // #ifndef BOOT_STAGE2_LOAD_FILE_H
