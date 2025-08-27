#ifndef ARCH_X86_BOCHS_H
#define ARCH_X86_BOCHS_H

#include "kernel/arch/x86/io.h"

static inline void bochs_break() {
  port_out_16(0x8A00, 0x8A00);
  port_out_16(0x8A00, 0x8A00);
}

static inline void bochs_e9_pchar(char c) {
  port_out_8(0xE9, c);
}

#endif // #ifndef ARCH_X86_BOCHS_H
