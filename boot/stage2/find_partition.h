#ifndef BOOT_STAGE2_FIND_PARTITION_H
#define BOOT_STAGE2_FIND_PARTITION_H

#include "boot/stage2/error.h"
#include "core_lib/mbr.h"

error_t find_partition(int drive, MBR_partition_table_t **pt, int *partition);

#endif // #ifndef BOOT_STAGE2_FIND_PARTITION_H
