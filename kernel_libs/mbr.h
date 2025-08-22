#ifndef LIB_MBR_H
#define LIB_MBR_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint8_t boot_indicator;
  uint8_t starting_head;
  uint8_t starting_sector;
  uint8_t starting_cylinder;
  uint8_t partition_type;
  uint8_t ending_head;
  uint8_t ending_sector;
  uint8_t ending_cylinder;
  uint32_t starting_LBA;
  uint32_t ending_LBA;
} __attribute__((packed)) MBR_partition_entry_t;

typedef struct {
  uint32_t disk_id;
  uint16_t reserved;
  MBR_partition_entry_t entries[4];
} __attribute__((packed)) MBR_partition_table_t;

static const size_t k_MBR_partition_table_offset = 440;
static const size_t k_MBR_pinguinOS_partition_type = 228;
static const size_t k_MBR_signature_offset = 510;
static const uint16_t k_MBR_signature = 0xAA55;

#endif // #ifndef LIB_MBR_H
