#include <gtest/gtest.h>

extern "C" {
#include "kernel/config.h"
#include "kernel/mem/util.h"
}

TEST(MemUtilTest, AlginUpTest) {
  size_t expected = PAGE_SIZE * 12345;
  size_t unaligned = (size_t)expected - 123 % PAGE_SIZE;
  void *aligned = page_size_align_up((void *)unaligned);
  EXPECT_EQ((void *)expected, aligned);
}

TEST(MemUtilTest, AlginDownTest) {
  size_t expected = PAGE_SIZE * 12345;
  size_t unaligned = (size_t)expected + 123 % PAGE_SIZE;
  void *aligned = page_size_align_down((void *)unaligned);
  EXPECT_EQ((void *)expected, aligned);
}

TEST(MemUtilTest, MaxLowerPowerOf2) {
  EXPECT_EQ(max_lower_power_of_2(10), 8);
  EXPECT_EQ(max_lower_power_of_2(8), 8);
  EXPECT_EQ(max_lower_power_of_2(228), 128);
}

TEST(MemUtilTest, MinUpperPowerOf2) {
  EXPECT_EQ(min_upper_power_of_2(10), 16);
  EXPECT_EQ(min_upper_power_of_2(8), 8);
  EXPECT_EQ(min_upper_power_of_2(228), 256);
}

TEST(MemUtilTest, Log2Upper) {
  EXPECT_EQ(log2_lower(10), 3);
  EXPECT_EQ(log2_lower(8), 3);
  EXPECT_EQ(log2_lower(228), 7);
}

TEST(MemUtilTest, Log2Lower) {
  EXPECT_EQ(log2_upper(10), 4);
  EXPECT_EQ(log2_upper(8), 3);
  EXPECT_EQ(log2_upper(228), 8);
}
