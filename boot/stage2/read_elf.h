#ifndef BOOT_STAGE2_RUN_ELF_H
#define BOOT_STAGE2_RUN_ELF_H

#include "boot/stage2/error.h"

error_t read_elf(char *name, void **main);

#endif // #ifndef BOOT_STAGE2_RUN_ELF_H
