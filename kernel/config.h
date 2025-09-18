#ifndef KERNEL_BUILDCONFIG_H
#define KERNEL_BUILDCONFIG_H

#include <stdint.h>

// TODO: Define this in build system
#define ARCH_X86 1

#define LOGS_TO_STDOUT 0

#define KERNEL_SHELL_BUFFER_SIZE 512

static const char *kernel_shell_prompt = "> ";

#define ENABLE_LOGS 1

#define USE_DEBUG_LOGS 1
#define USE_INFO_LOGS 1
#define USE_WARNING_LOGS 1
#define USE_ERROR_LOGS 1
#define USE_FATAL_LOGS 1

// Make sure that the following equalation is true
// 2^(BUDDY_LAYERS - 1) * PAGE_SIZE = POINTER_MAX
#define POINTER_MAX UINT32_MAX
#define PAGE_SIZE 4096
#define BUDDY_LAYERS 21

#if (1 << (BUDDY_LAYERS - 1)) * PAGE_SIZE != POINTER_MAX + 1
#error "Invalid configuration"
#endif

#if POINTER_MAX + 1 < 4294967296
#error "Invalid configuration"
#endif

#endif // #ifndef KERNEL_BUILDCONFIG_H
