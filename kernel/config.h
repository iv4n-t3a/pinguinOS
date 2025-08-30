#ifndef KERNEL_BUILDCONFIG_H
#define KERNEL_BUILDCONFIG_H

// TODO: Define this in build system
#define ARCH_X86 1

#define LOGS_TO_STDOUT 0

#define KERNEL_SHELL_BUFFER_SIZE 512

static const char *kernel_shell_prompt = "> ";

#define ENABLE_LOGS 1

#define USE_DEBUG_LOGS 0
#define USE_INFO_LOGS 1
#define USE_WARNING_LOGS 1
#define USE_ERROR_LOGS 1
#define USE_FATAL_LOGS 1

#endif // #ifndef KERNEL_BUILDCONFIG_H
