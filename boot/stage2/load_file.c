#include "boot/stage2/load_file.h"

#include "boot/stage2/load_sectors.h"
#include "core_lib/fat16.h"
#include "core_lib/string.h"

#define FAT_BUFFER_SIZE 512
#define ROOT_DIR_BUFFER_SIZE 512

typedef uint16_t cluster_t;

size_t loaded_fat_segment;
int load_file_drive;

uint8_t vbr_buffer[512];
FAT16_direntry_t root_dir[512];
cluster_t fat_buffer[FAT_BUFFER_SIZE / sizeof(cluster_t)];

load_file_error_t init_fat(int drive, MBR_partition_entry_t *entry) {
  loaded_fat_segment = -1;
  load_file_drive = drive;

  int err = load_sectors(drive, vbr_buffer, entry->starting_LBA, 1);

  if (err != LOAD_SECTORS_SUCCESS) {
    return err;
  }

  if (*(uint16_t *)(vbr_buffer + 510) != 0xAA55) {
    return INIT_FAT_SIGNATURE_VERIFICATION_FAILED;
  }

  FAT16_BPB_t *bpb = (FAT16_BPB_t *)vbr_buffer;

  if (FAT_BUFFER_SIZE % bpb->bytes_per_sector != 0) {
    return INIT_FAT_INCOMPATIBLE_FAT_BUFFER_SIZE;
  }

  return load_sectors(load_file_drive, root_dir, fat16_root_dir_lba(bpb),
                      fat16_root_dir_sectors(bpb));
}

int next_cluster(FAT16_BPB_t *bpb, cluster_t *cluster) {
  size_t fat_segment = fat16_fat_lba(bpb) + *cluster / FAT_BUFFER_SIZE;
  size_t location_in_fat_buffer = *cluster % FAT_BUFFER_SIZE;

  if (fat_segment != loaded_fat_segment) {
    int err = load_sectors(load_file_drive, fat_buffer, fat_segment,
                           FAT_BUFFER_SIZE / bpb->bytes_per_sector);
    if (err) {
      return err;
    }
  }

  *cluster = fat_buffer[location_in_fat_buffer];
  return 0;
}

load_file_error_t load_file(uint8_t *buffer, char *name) {
  if (strlen(name) != FAT16_FILENAME_LEN) {
    return LOAD_FILE_INVALID_FILE_NAME;
  }

  FAT16_BPB_t *bpb = (FAT16_BPB_t *)vbr_buffer;

  FAT16_direntry_t *file = nullptr;

  for (size_t i = 0; i < bpb->root_dir_entries; ++i) {
    if (strcmp(root_dir[i].filename, name, FAT16_FILENAME_LEN)) {
      file = &root_dir[i];
    }
  }

  if (!file) {
    return LOAD_FILE_FILE_NOT_FOUND;
  }

  // FAT16 don't use first_cluster_high field
  cluster_t cluster = file->first_cluster_low;

  while (cluster < k_fat16_final_cluster) {
    int err = load_sectors(load_file_drive, buffer,
                           fat16_get_cluster_lba(bpb, cluster),
                           bpb->sectors_per_cluster);
    buffer += bpb->sectors_per_cluster;

    if (err) {
      return err;
    }

    err = next_cluster(bpb, &cluster);

    if (err) {
      return err;
    }
  }

  return LOAD_FILE_SUCCESS;
}
