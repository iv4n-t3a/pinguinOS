#ifndef BOOT_STAGE2_FAT_H
#define BOOT_STAGE2_FAT_H

#include <stddef.h>
#include <stdint.h>

#include "boot/stage2/error.h"
#include "core_lib/fat16.h"
#include "core_lib/mbr.h"

typedef uint16_t FAT_cluster_t;

typedef struct {
  FAT_cluster_t cluster;
  size_t pos_in_cluster;
  size_t bytes_left;
} FAT_file_t;

error_t FAT_init(int drive, MBR_partition_entry_t *entry);

error_t FAT_open(FAT_file_t *file, char *name);

error_t FAT_read(FAT_file_t *file, uint8_t *buffer, size_t bytes);

#endif // #ifndef BOOT_STAGE2_LOAD_FAT_H
