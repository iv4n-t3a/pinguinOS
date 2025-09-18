#include "core_lib/boot_params.h"
#include "kernel/arch/debug/keyboard.h"

void arch_init(boot_params_t params) { run_keyboard_emulator(); }
