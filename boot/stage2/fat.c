#include "boot/stage2/fat.h"

#include "boot/stage2/build_config.h"
#include "boot/stage2/read_disk.h"
#include "core_lib/fat16.h"
#include "core_lib/math.h"
#include "core_lib/string.h"

size_t loaded_fat_segment;
int current_drive;

FAT16_BPB_t *bpb;
uint8_t vbr_buffer[VBR_BUFFER_SIZE];
uint8_t transfer_buffer[FAT_TRANSFER_BUFFER_SIZE];
FAT16_direntry_t root_dir[ROOT_DIR_BUFFER_SIZE];
FAT_cluster_t fat_buffer[FAT_BUFFER_SIZE / sizeof(FAT_cluster_t)];

error_t FAT_init(int drive, MBR_partition_entry_t *entry) {
  loaded_fat_segment = -1;
  current_drive = drive;

  ERR_HANDLE_SUBROUTINE(read_disk(drive, vbr_buffer, entry->starting_LBA, 1));

  if (*(uint16_t *)(vbr_buffer + k_MBR_signature_offset) != k_MBR_signature) {
    return ERR_SIGNATURE_VERIFICATION_FAILED;
  }

  bpb = (FAT16_BPB_t *)vbr_buffer;

  if (FAT_BUFFER_SIZE % bpb->bytes_per_sector != 0) {
    return ERR_INCOMPATIBLE_BUFFER_SIZE;
  }

  memset(root_dir, 0, sizeof(root_dir));
  return read_disk(current_drive, root_dir, fat16_root_dir_lba(bpb),
                   fat16_root_dir_sectors(bpb));
}

error_t FAT_next_cluster(FAT16_BPB_t *bpb, FAT_cluster_t *cluster) {
  size_t fat_segment = fat16_fat_lba(bpb) + *cluster / FAT_BUFFER_SIZE;
  size_t location_in_fat_buffer = *cluster % FAT_BUFFER_SIZE;

  if (fat_segment != loaded_fat_segment) {
    ERR_HANDLE_SUBROUTINE(read_disk(current_drive, fat_buffer, fat_segment, 1));
  }

  *cluster = fat_buffer[location_in_fat_buffer];
  return ERR_SUCCESS;
}

error_t FAT_open(FAT_file_t *file, char *name) {
  if (strlen(name) != FAT16_FILENAME_LEN) {
    return ERR_INVALID_FILE_NAME;
  }

  FAT16_direntry_t *dirent = nullptr;

  for (size_t i = 0; i < bpb->root_dir_entries; ++i) {
    if (strcmp(root_dir[i].filename, name, FAT16_FILENAME_LEN)) {
      dirent = &root_dir[i];
    }
  }

  if (!dirent) {
    return ERR_FILE_NOT_FOUND;
  }

  // FAT16 don't use first_cluster_high field
  file->cluster = dirent->first_cluster_low;
  file->pos_in_cluster = 0;
  file->bytes_left = dirent->file_size;

  return ERR_SUCCESS;
}

error_t FAT_read(FAT_file_t *file, uint8_t *buffer, size_t bytes_to_read) {
  if (fat16_bytes_per_cluster(bpb) > FAT_TRANSFER_BUFFER_SIZE) {
    return ERR_INCOMPATIBLE_BUFFER_SIZE;
  }

  if (bytes_to_read > file->bytes_left) {
    bytes_to_read = file->bytes_left;
  }

  if (file->pos_in_cluster == fat16_bytes_per_cluster(bpb)) {
    file->pos_in_cluster = 0;
    ERR_HANDLE_SUBROUTINE(FAT_next_cluster(bpb, &file->cluster));
  }

  if (file->pos_in_cluster != 0) {
    int lba_offset = file->pos_in_cluster / bpb->bytes_per_sector;
    int target_lba = fat16_get_cluster_lba(bpb, file->cluster) + lba_offset;
    int sectors_to_read = bpb->sectors_per_cluster - lba_offset;
    ERR_HANDLE_SUBROUTINE(
        read_disk(current_drive, transfer_buffer, target_lba, sectors_to_read));

    int bytes_read =
        MIN(fat16_bytes_per_cluster(bpb) - file->pos_in_cluster, bytes_to_read);
    memcpy(buffer, transfer_buffer + file->pos_in_cluster, bytes_read);
    file->bytes_left -= bytes_read;
    bytes_to_read -= bytes_read;
    buffer += bytes_read;
    file->pos_in_cluster += bytes_read;

    if (!bytes_to_read) {
      return ERR_SUCCESS;
    }

    ERR_HANDLE_SUBROUTINE(FAT_next_cluster(bpb, &file->cluster));

    file->pos_in_cluster = 0;
  }

  while (bytes_to_read / fat16_bytes_per_cluster(bpb) &&
         file->cluster < k_fat16_final_cluster) {
    ERR_HANDLE_SUBROUTINE(read_disk(current_drive, buffer,
                                    fat16_get_cluster_lba(bpb, file->cluster),
                                    bpb->sectors_per_cluster));

    file->bytes_left -= fat16_bytes_per_cluster(bpb);
    bytes_to_read -= fat16_bytes_per_cluster(bpb);
    buffer += fat16_bytes_per_cluster(bpb);

    ERR_HANDLE_SUBROUTINE(FAT_next_cluster(bpb, &file->cluster));
  }

  if (bytes_to_read) {
    ERR_HANDLE_SUBROUTINE(read_disk(current_drive, transfer_buffer,
                                    fat16_get_cluster_lba(bpb, file->cluster),
                                    bpb->sectors_per_cluster));

    memcpy(buffer, transfer_buffer, bytes_to_read);
    file->bytes_left -= bytes_to_read;
    file->pos_in_cluster += bytes_to_read;
  }

  return ERR_SUCCESS;
}
