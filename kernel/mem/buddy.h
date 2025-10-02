#ifndef KERNEL_MEM_BUDDY_H
#define KERNEL_MEM_BUDDY_H

#include "kernel/mem/freelist.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
  // Segments use bitmap. Stored in in the following order
  // [ X0112222 33333333 44444444 44444444 ... ]
  uint8_t *bitmap;

  // Free-list for each block size managed by allocator
  freelist_t *freelist;

  // Count of layers. Each layer corresponds for power of to block-size.
  // Zero layer contains the largest root segment
  size_t buddy_layers;

  size_t blocks_count;

  size_t root_block_size;

  void *begin;
} buddy_alloc_t;

buddy_alloc_t *buddy_init(void *begin, void *end);

void *buddy_alloc(buddy_alloc_t *alloc, size_t pages);

void buddy_free(buddy_alloc_t *alloc, void *ptr);

#endif // #ifndef KERNEL_MEM_BUDDY_H
