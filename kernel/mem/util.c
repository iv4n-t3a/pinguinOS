#include "kernel/mem/util.h"
#include "kernel/config.h"

void *page_size_align_up(void *ptr) {
  return ptr + (PAGE_SIZE - (size_t)ptr % PAGE_SIZE);
}

void *page_size_align_down(void *ptr) { return ptr - (size_t)ptr % PAGE_SIZE; }

size_t min_upper_power_of_2(size_t size) {
  size_t p = 0;

  while ((1ull << p) < size) {
    p += 1;
  }

  return 1ull << p;
}

size_t max_lower_power_of_2(size_t size) {
  size_t p = 0;

  while ((1ull << (p + 1)) <= size) {
    p += 1;
  }

  return 1ull << p;
}

size_t log2_upper(size_t size) {
  size_t p = 0;

  while ((1ull << p) < size) {
    p += 1;
  }

  return p;
}

size_t log2_lower(size_t size) {
  size_t p = 0;

  while ((1ull << (p + 1)) <= size) {
    p += 1;
  }

  return p;
}

size_t pages_count(void *begin, void *end) {
  begin = page_size_align_up(begin);
  end = page_size_align_down(end);
  return (end - begin) / PAGE_SIZE;
}
