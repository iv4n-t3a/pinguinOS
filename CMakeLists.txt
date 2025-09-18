cmake_minimum_required(VERSION 3.31)

project("Pinguin OS" ASM C CXX)

if("${ARCH}" STREQUAL "x86")
  add_subdirectory(boot/stage1)
  add_subdirectory(boot/stage2)
endif()

add_subdirectory(core_lib)
add_subdirectory(kernel)

if("${TARGET}" STREQUAL "TESTS")
  add_subdirectory(tests)
endif()
