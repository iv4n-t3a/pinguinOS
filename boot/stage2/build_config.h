#ifndef BOOT_STAGE2_BUILD_CONFIG_H
#define BOOT_STAGE2_BUILD_CONFIG_H

#define MBR_BUFFER_SIZE 512
#define VBR_BUFFER_SIZE 512

#define DEFAULT_SECTOR_SIZE 512

// Must be greater than sector size
#define READ_DISK_MEDIATOR_BUFFER_SIZE DEFAULT_SECTOR_SIZE

// Must be greater than cluster size in bytes
#define FAT_TRANSFER_BUFFER_SIZE (4 * DEFAULT_SECTOR_SIZE)

// In bytes
#define FAT_BUFFER_SIZE 512

// In direntries (each 32 byte)
#define ROOT_DIR_BUFFER_SIZE 512

#endif // #ifndef BOOT_STAGE2_BUILD_CONFIG_H
