#ifndef KERNEL_LIBS_STDIO_H
#define KERNEL_LIBS_STDIO_H

///////////////////////////////////////////////////////////////////////////////
////// The following symbols shall be defined in architecture specific dirs

// For interaction with user
void kpchar(char);

// For kernel logs
void dpchar(char);

///////////////////////////////////////////////////////////////////////////////
////// core_lib/stdio.h wrappers

void kputstr(const char *str);

void kprintf(const char *str, ...);

#endif // #ifndef KERNEL_LIBS_STDIO_H
