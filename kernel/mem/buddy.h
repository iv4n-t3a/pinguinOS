#ifndef KERNEL_MEM_BUDDY_H
#define KERNEL_MEM_BUDDY_H

#include <stddef.h>
#include <stdint.h>

#include "core_lib/boot_params.h"
#include "kernel/config.h"

#define BUDDY_PAGES_COUNT (POINTER_MAX / PAGE_SIZE)
#define BUDDY_MAX_BLOCK_SIZE BUDDY_PAGES_COUNT

// bitmap size in bytes
#define BUDDY_BITMAP_SIZE (2 * BUDDY_PAGES_COUNT / 8)

typedef struct {
  // Buddies are stored in in the following order
  // [ X0112222 33333333 44444444 44444444 ... ]
  // X is padding. Each digit indicates number of buddy bit
  // correspondes to.
  uint8_t bitmap[BUDDY_BITMAP_SIZE];
  void *freelist[BUDDY_LAYERS];
} buddy_alloc_t;

void buddy_init(buddy_alloc_t *alloc, mementry_t *reserved_regions,
                size_t reserved_regions_count, size_t pages_availible);

void *buddy_alloc(buddy_alloc_t *alloc, size_t pages);

#endif // #ifndef KERNEL_BUDDY_H
