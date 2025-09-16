set(SOURCES
    bios_calls.S
    detect_memory.c
    entry.S
    error.c
    fat.c
    find_partition.c
    main.c
    read_disk.c
    read_elf.c
    stdio.c)

set(HEADERS
    bios_calls.h
    build_config.h
    compiler.h
    detect_memory.h
    error.h
    fat.h
    find_partition.h
    read_disk.h
    read_elf.h
    stdio.h)

add_executable(stage2.bin ${SOURCES} ${HEADERS})

set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/boot/stage2/stage2.ld")

target_include_directories(stage2.bin PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(stage2.bin core_lib)

target_link_options(stage2.bin PRIVATE "LINKER:-T${LINKER_SCRIPT}")
target_link_options(stage2.bin PRIVATE "LINKER:-Map=${PROJECT_SOURCE_DIR}/stage2.map")

set_target_properties(stage2.bin PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")
