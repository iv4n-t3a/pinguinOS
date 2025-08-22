#ifndef BOOT_STAGE2_ERROR_LOG_H
#define BOOT_STAGE2_ERROR_LOG_H

#include "kernel_libs/vga.h"

static inline void error_log(const char* msg, int error_code) {
    terminal_writestring("STAGE2: ");
    terminal_writestring(msg);
    terminal_writestring("\n    Error code: ");

    if (error_code < 10) {
      terminal_putchar(error_code + '0');
    } else {
      terminal_putchar(error_code / 10 + '0');
      terminal_putchar(error_code % 10 + '0');
    }
}

#endif  // #ifndef BOOT_STAGE2_ERROR_LOG_H
