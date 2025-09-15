#ifndef BOOT_STAGE2_DETECT_MEMORY_H
#define BOOT_STAGE2_DETECT_MEMORY_H

#include <stddef.h>

#include "boot/stage2/error.h"
#include "core_lib/boot_params.h"

error_t detect_memory(boot_params_t *);

#endif // #ifndef BOOT_STAGE2_DETECT_MEMORY_H
