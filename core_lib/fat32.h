#ifndef CORE_LIB_FAT32_H
#define CORE_LIB_FAT32_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t bootjmp[3];

  const char OEM_identifier[8];
  uint16_t bytes_per_sector;
  uint8_t sectors_per_cluster;
  uint16_t reserved_sectors;
  uint8_t FAT_count;
  uint8_t root_dir_entries;
  uint16_t sectors_per_volume_u16;
  uint8_t media_descriptor_type;
  uint16_t sectors_per_FAT_u16;
  uint16_t sectors_per_track;
  uint16_t number_of_heads;
  uint32_t part_begin_lba;
  uint32_t sectors_per_volume_u32;

  uint32_t sectors_per_FAT;
  uint16_t flags;
  uint16_t FAT_version_number;
  uint32_t root_cluster;
  uint16_t FSInfo_sector;
  uint16_t backup_boot_sector;
  uint8_t reserved[12];
  uint8_t drive_number;
  uint8_t windows_nt_flags;
  uint8_t signature; /* must be 0x28 or 0x29 */
  uint32_t volume_id;
  char volume_label[11];
  char system_identifier[8];
} __attribute__((packed)) FAT32_BPB_t;

typedef struct {

} FAT32_DIR_entry_t;

typedef struct {

} FAT32_FSInfo_t;

static inline size_t fat32_sectors_per_volume(FAT32_BPB_t *bpb) {
  return bpb->sectors_per_volume_u16 ? bpb->sectors_per_volume_u16
                                     : bpb->sectors_per_volume_u32;
}

static inline size_t fat32_fat_lba(FAT32_BPB_t *bpb) {
  return bpb->part_begin_lba + bpb->reserved_sectors;
}

static inline size_t fat32_root_dir_sectors(FAT32_BPB_t *bpb) { return 0; }

static inline size_t fat32_data_lba(FAT32_BPB_t *bpb) {
  return fat32_fat_lba(bpb) + bpb->sectors_per_FAT * bpb->FAT_count +
         fat32_root_dir_sectors(bpb);
}

static inline size_t fat32_data_sectors(FAT32_BPB_t *bpb) {
  return fat32_sectors_per_volume(bpb) - fat32_data_lba(bpb);
}

static inline size_t fat32_total_clusters(FAT32_BPB_t *bpb) {
  return fat32_data_sectors(bpb) / bpb->sectors_per_cluster;
}

static inline size_t fat32_get_cluster_lba(FAT32_BPB_t *bpb, size_t cluster) {
  return fat32_data_lba(bpb) + (cluster - 2) * bpb->sectors_per_cluster;
}

#endif // #ifndef LIB_FAT32_H
