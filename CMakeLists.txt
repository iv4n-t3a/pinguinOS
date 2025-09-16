cmake_minimum_required(VERSION 3.31)

project("Pinguin OS" ASM C)

add_subdirectory(boot/stage1)
add_subdirectory(boot/stage2)
add_subdirectory(core_lib)
add_subdirectory(kernel)
