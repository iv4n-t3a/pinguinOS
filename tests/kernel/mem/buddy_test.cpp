#include <gtest/gtest.h>

extern "C" {
#include "core_lib/boot_params.h"
#include "kernel/config.h"
#include "kernel/mem/buddy.h"
}

TEST(BuddyUnitTests, SanityCheck) {
  buddy_alloc_t alloc;

  mementry_t free_regions[] = {
      {2, 100},
  };

  buddy_init(&alloc, free_regions, sizeof(free_regions) / sizeof(mementry_t));

  void *ptr = buddy_alloc(&alloc, 10);
  EXPECT_EQ(ptr, (void *)(2 * PAGE_SIZE));
  buddy_free(&alloc, ptr);
}
