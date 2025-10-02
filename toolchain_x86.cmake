set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(CMAKE_C_COMPILER "${PROJECT_SOURCE_DIR}/third_party/bin/i686-elf-gcc")
set(CMAKE_ASM_COMPILER "${PROJECT_SOURCE_DIR}/third_party/bin/i686-elf-gcc")

set(CMAKE_C_FLAGS "-ffreestanding")
add_link_options("-nostdlib" "-nostartfiles")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
