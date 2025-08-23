#ifndef BOOT_STAGE2_READ_DISK_H
#define BOOT_STAGE2_READ_DISK_H

#include <stddef.h>
#include <stdint.h>

#include "boot/stage2/error.h"

error_t read_disk(int drive, void *dst, uint64_t lba, uint16_t sectors);

#endif // #ifndef BOOT_STAGE2_READ_DISK_H
