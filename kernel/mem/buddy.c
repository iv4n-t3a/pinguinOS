#include "kernel/mem/buddy.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core_lib/boot_params.h"
#include "kernel/config.h"

////////////////////////////////////////////////////////////////////////////////
// Helpers declaration:

void buddy_init_region(buddy_alloc_t *alloc, void *ptr, size_t size);

bool buddy_get(buddy_alloc_t *alloc, size_t buddy, size_t index);

// Mark and unmark functions updates bit for given buddy layers and all upper.
// The highest updated layer is returned.
size_t buddy_mark(buddy_alloc_t *alloc, size_t buddy, size_t index);
size_t buddy_unmark(buddy_alloc_t *alloc, size_t buddy, size_t index);

size_t block_size(size_t buddy);
size_t best_fit_buddy(size_t pages);

void *to_pointer(size_t buddy, size_t index);
size_t to_index(size_t buddy, void *pointer);

void *get_next_free(void *free_block);
void set_next_free(void *free_block, void *next_free);

////////////////////////////////////////////////////////////////////////////////
// Allocator functions:

void buddy_init(buddy_alloc_t *alloc, mementry_t *free_regions,
                size_t free_regions_count) {
  // Empty freelist
  for (size_t i = 0; i < BUDDY_LAYERS; ++i) {
    alloc->freelist[i] = NULL;
  }

  // Mark all pages as used
  for (size_t i = 0; i < BUDDY_BITMAP_SIZE; ++i) {
    alloc->bitmap[i] = 0xFF;
  }

  for (size_t i = 0; i < free_regions_count; ++i) {
    buddy_init_region(alloc, (void *)free_regions[i].base,
                      free_regions[i].length);
  }
}

void *buddy_alloc(buddy_alloc_t *alloc, size_t pages) {
  for (int buddy = best_fit_buddy(pages); buddy >= 0; --buddy) {
    void *block = alloc->freelist[buddy];

    if (block == NULL) {
      continue;
    }

    alloc->freelist[buddy] = get_next_free(block);
    buddy_mark(alloc, buddy, to_index(buddy, block));
    return block;
  }

  return NULL;
}

void buddy_free(buddy_alloc_t *alloc, void *ptr) {
  int buddy = BUDDY_LAYERS - 1;

  while (!buddy_get(alloc, buddy, to_index(buddy, ptr))) {
    buddy -= 1;
  }

  buddy = buddy_unmark(alloc, buddy, to_index(buddy, ptr));

  set_next_free(ptr, alloc->freelist[buddy]);
  alloc->freelist[buddy] = ptr;
}

////////////////////////////////////////////////////////////////////////////////
// Helpers definition:

void buddy_init_region(buddy_alloc_t *alloc, void *ptr, size_t size) {
  // Page size align
  size += (size_t)ptr % PAGE_SIZE;
  ptr -= (size_t)ptr % PAGE_SIZE;

  size_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
  size_t buddy = best_fit_buddy(pages);
  size_t index = to_index(buddy, ptr);

  if (pages < block_size(buddy)) {
    buddy += 1;
  }

  while (pages) {
    buddy_unmark(alloc, buddy, index);
    pages -= block_size(buddy);
    buddy += 1;
    index = (index + 1) * 2;
  }
}

// Returns index of 8-bit mask containing value for desired index in buddy
size_t mask_index(size_t buddy, size_t index) {
  if (index < 3) {
    return 0;
  }

  size_t buddy_start =
      1 << (buddy - 3); // Skip first 3 buddies as they occupy first byte
  return buddy_start + index / sizeof(uint8_t);
}

// Returns index of required bit in mask_index result
size_t bit_index(size_t buddy, size_t index) {
  if (buddy < 3) { // 1-bit padding
    return index - 1;
  }

  return index % sizeof(uint8_t);
}

// Returns mask for required bit in mask_index result
uint8_t bit_mask(size_t buddy, size_t index) {
  return 1 << bit_index(buddy, index);
}

bool buddy_get(buddy_alloc_t *alloc, size_t buddy, size_t index) {
  return alloc->bitmap[mask_index(buddy, index)] & bit_mask(buddy, index);
}

size_t buddy_mark(buddy_alloc_t *alloc, size_t buddy, size_t index) {
  alloc->bitmap[mask_index(buddy, index)] |= bit_mask(buddy, index);

  // TODO: Check if compiler optimize this recursion. Turn into cycle if needed
  if (buddy != 0 && buddy_get(alloc, buddy, index ^ 1)) {
    return buddy_mark(alloc, buddy - 1, index / 2);
  }

  return buddy;
}

size_t buddy_unmark(buddy_alloc_t *alloc, size_t buddy, size_t index) {
  alloc->bitmap[mask_index(buddy, index)] &= ~bit_mask(buddy, index);

  if (buddy != 0 && !buddy_get(alloc, buddy, index ^ 1)) {
    return buddy_unmark(alloc, buddy - 1, index / 2);
  }

  return buddy;
}

size_t block_size(size_t buddy) { return (BUDDY_MAX_BLOCK_SIZE >> buddy) + 1; }

size_t best_fit_buddy(size_t pages) {
  for (size_t i = 1; block_size(i) < BUDDY_MAX_BLOCK_SIZE; ++i) {
    if (block_size(i) < pages) {
      return i - 1;
    }
  }
  return 0;
}

void *to_pointer(size_t buddy, size_t index) {
  return (void *)(block_size(buddy) * index * PAGE_SIZE);
}

size_t to_index(size_t buddy, void *pointer) {
  return (size_t)pointer / block_size(buddy) / PAGE_SIZE;
}

void *get_next_free(void *free_block) { return *(void **)free_block; }

void set_next_free(void *free_block, void *next_free) {
  *(void **)free_block = next_free;
}
