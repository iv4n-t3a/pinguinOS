#include "boot/stage2/find_partition.h"

#include "boot/stage2/build_config.h"
#include "boot/stage2/read_disk.h"
#include "core_lib/mbr.h"

uint8_t mbr_buffer[MBR_BUFFER_SIZE];

error_t find_partition(int drive, MBR_partition_table_t **pt, int *partition) {
  ERR_HANDLE_SUBROUTINE(read_disk(drive, mbr_buffer, 0, 1));

  if (*(uint16_t *)(mbr_buffer + k_MBR_signature_offset) != k_MBR_signature) {
    return NEW_ERR(ERR_SIGNATURE_VERIFICATION_FAILED);
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
    return NEW_ERR(ERR_PARTITION_NOT_FOUND);
  }

  return NEW_ERR(ERR_SUCCESS);
}
