set(TEST_SOURCES kernel/mem/buddy_test.cpp)

set(SOURCES ${PROJECT_SOURCE_DIR}/kernel/mem/buddy.c)

set(HEADERS ${PROJECT_SOURCE_DIR}/kernel/mem/buddy.h)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests ${TEST_SOURCES} ${SOURCES} ${HEADERS})

target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(unit_tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)
