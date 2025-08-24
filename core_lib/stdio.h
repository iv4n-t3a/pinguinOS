#ifndef CORE_LIB_STDIO_H
#define CORE_LIB_STDIO_H

#include <stdarg.h>

// The goal is to use the same code in bootloader, kernel shell, kernel logs and
// user space

void generic_putstr(void (*pchar)(char), const char *str);

void generic_printf(void (*pchar)(char), const char *fmt, va_list args);

#endif // #ifndef CORE_LIB_STDIO_H
