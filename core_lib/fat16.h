#ifndef CORE_LIB_FAT16_H
#define CORE_LIB_FAT16_H

#include <stddef.h>
#include <stdint.h>

#define FAT16_FILENAME_LEN 11
#define FAT16_FINAL_CLUSTER 0xFFF8
static const uint16_t k_fat16_final_cluster = 0xFFF8;

typedef struct {
  uint8_t bootjmp[3];

  uint8_t OEM_identifier[8];
  uint16_t bytes_per_sector;
  uint8_t sectors_per_cluster;
  uint16_t reserved_sectors;
  uint8_t FAT_count;
  uint16_t root_dir_entries;
  uint16_t sectors_per_volume_u16;
  uint8_t media_descriptor_type;
  uint16_t sectors_per_FAT;
  uint16_t sectors_per_track;
  uint16_t number_of_heads;
  uint32_t part_begin_lba;
  uint32_t sectors_per_volume_u32;

  uint8_t drive_number;
  uint8_t windows_nt_flags;
  uint8_t signature;
  uint32_t volume_id;
  char volume_label[11];
  char system_identifier[8];
} __attribute__((packed)) FAT16_BPB_t;

typedef struct {
  uint8_t filename[FAT16_FILENAME_LEN];
  uint8_t attr;
  uint8_t ntres;
  uint8_t creation_time_tenth;
  uint16_t creation_time;
  uint16_t creation_date;
  uint16_t last_access_date;
  uint16_t first_cluster_high;
  uint16_t write_time;
  uint16_t write_date;
  uint16_t first_cluster_low;
  uint32_t file_size;
} __attribute((packed)) FAT16_direntry_t;


static inline size_t fat16_sectors_per_volume(FAT16_BPB_t *bpb) {
  return bpb->sectors_per_volume_u16 ? bpb->sectors_per_volume_u16
                                     : bpb->sectors_per_volume_u32;
}

static inline size_t fat16_fat_lba(FAT16_BPB_t *bpb) {
  return bpb->part_begin_lba + bpb->reserved_sectors;
}

static inline size_t fat16_root_dir_lba(FAT16_BPB_t *bpb) {
  return fat16_fat_lba(bpb) + bpb->FAT_count * bpb->sectors_per_FAT;
}

static inline size_t fat16_root_dir_sectors(FAT16_BPB_t *bpb) {
  return (bpb->root_dir_entries * 32 + bpb->bytes_per_sector - 1) /
         bpb->bytes_per_sector;
}

static inline size_t fat16_data_lba(FAT16_BPB_t *bpb) {
  return fat16_fat_lba(bpb) + bpb->sectors_per_FAT * bpb->FAT_count +
         fat16_root_dir_sectors(bpb);
}

static inline size_t fat16_data_sectors(FAT16_BPB_t *bpb) {
  return fat16_sectors_per_volume(bpb) - fat16_data_lba(bpb);
}

static inline size_t fat16_total_clusters(FAT16_BPB_t *bpb) {
  return fat16_data_sectors(bpb) / bpb->sectors_per_cluster;
}

static inline size_t fat16_get_cluster_lba(FAT16_BPB_t *bpb, size_t cluster) {
  return fat16_data_lba(bpb) + (cluster - 2) * bpb->sectors_per_cluster;
}

static inline size_t fat16_bytes_per_cluster(FAT16_BPB_t *bpb) {
  return bpb->bytes_per_sector * bpb->sectors_per_cluster;
}

#endif // #ifndef CORE_LIB_FAT16_H
