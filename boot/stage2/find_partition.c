#include "boot/stage2/find_partition.h"

#include "boot/stage2/load_sectors.h"
#include "kernel_libs/mbr.h"

uint8_t mbr_buffer[512];

find_partition_error_t find_partition(int drive, MBR_partition_table_t **pt,
                                      int *partition) {
  load_sectors_error_t ls_err = load_sectors(drive, mbr_buffer, 0, 1);

  if (ls_err != 0) {
    return ls_err;
  }

  if (*(uint16_t *)(mbr_buffer + k_MBR_signature_offset) != k_MBR_signature) {
    return FIND_PARTITION_MBR_SIGNATURE_NOT_MATCH;
  }

  *pt = (MBR_partition_table_t *)(mbr_buffer + k_MBR_partition_table_offset);

  *partition = 5;

  for (int i = 0; i < 4; ++i) {
    if ((*pt)->entries[i].partition_type == k_MBR_pinguinOS_partition_type) {
      *partition = i;
      break;
    }
  }

  if (*partition == 5) {
    return FIND_PARTITION_NOT_FOUND;
  }

  return FIND_PARTITION_SUCCESS;
}
