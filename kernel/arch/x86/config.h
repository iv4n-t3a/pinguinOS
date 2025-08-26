#ifndef KERNEL_ARCH_X86_CONFIG_H
#define KERNEL_ARCH_X86_CONFIG_H

// x86 specific config

#include <stdint.h>

#define PIC_MASTER_OFFSET 0x20
#define PIC_SLAVE_OFFSET 0x28

static const uint16_t k_pic_mask = 0b1111'1001;

static const int k_terminal_height = 25;
static const int k_terminal_width = 80;

#endif // #ifndef KERNEL_ARCH_X86_CONFIG_H
