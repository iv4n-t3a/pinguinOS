set(SOURCES
    drivers/init.c
    drivers/ps2_keyboard.c
    libs/kernel_log.c
    libs/stdio.c
    log_boot_params.c
    mem/buddy.c
    main.c
    shell/shell.c)

set(HEADERS
    compiler.h
    config.h
    drivers/init.h
    drivers/keyboard.h
    drivers/ps2_keyboard.h
    libs/kernel_log.h
    libs/kernel_panic.h
    libs/stdio.h
    mem/buddy.h
    log_boot_params.h
    shell/shell.h)

if("${ARCH}" STREQUAL "x86")
  list(
    APPEND
    SOURCES
    arch/x86/gdt.c
    arch/x86/idt.c
    arch/x86/init.c
    arch/x86/isr.c
    arch/x86/pic.c
    arch/x86/ps2_keyboard.c
    arch/x86/vga.c)

  list(
    APPEND
    HEADERS
    arch/x86/bochs.h
    arch/x86/config.h
    arch/x86/gdt.h
    arch/x86/idt.h
    arch/x86/io.h
    arch/x86/isr.h
    arch/x86/pic.h)
else()
  message(FATAL_ERROR "Unsupported architecture ${ARCH}")
endif()

add_executable(kernel.elf ${SOURCES} ${HEADERS})

set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/kernel/kernel.ld")

target_include_directories(kernel.elf PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(kernel.elf core_lib)

target_link_options(kernel.elf PRIVATE "LINKER:-T${LINKER_SCRIPT}")
target_link_options(stage2.bin PRIVATE
                    "LINKER:-Map=${PROJECT_SOURCE_DIR}/kernel.map")

set_target_properties(kernel.elf PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")
