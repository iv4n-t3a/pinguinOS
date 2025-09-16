cmake_minimum_required(VERSION 3.31)

project("Pinguin OS" ASM C CXX)

if(${TARGET} STREQUAL BOOTABLE)
  add_subdirectory(boot/stage1)
  add_subdirectory(boot/stage2)
  add_subdirectory(core_lib)
  add_subdirectory(kernel)
elseif(${TARGET} STREQUAL TESTS)
  add_subdirectory(tests)
else()
  message(FATAL_ERROR "Unsupported target ${TARGET}")
endif()
