set(SOURCES stdio.c string.c vga.c)

set(HEADERS
    boot_params.h
    compiler.h
    elf.h
    fat16.h
    fat32.h
    math.h
    mbr.h
    stdio.h
    string.h
    vga.h)

add_library(core_lib STATIC ${SOURCES} ${HEADERS})

target_include_directories(core_lib PRIVATE ${PROJECT_SOURCE_DIR})
