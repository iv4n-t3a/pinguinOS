#ifndef BOOT_STAGE2_FIND_PARTITION_H
#define BOOT_STAGE2_FIND_PARTITION_H

#include "core_lib/mbr.h"

typedef enum {
  FIND_PARTITION_SUCCESS = 0,
  FIND_PARTITION_DAPACK_INIT_ERROR = 1,
  FIND_PARTITION_BIOS_CALL_ERROR = 2,
  FIND_PARTITION_MBR_SIGNATURE_NOT_MATCH,
  FIND_PARTITION_NOT_FOUND,
} find_partition_error_t;

find_partition_error_t find_partition(int drive, MBR_partition_table_t **pt,
                                      int *partition);

#endif // #ifndef BOOT_STAGE2_FIND_PARTITION_H
