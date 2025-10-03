#ifndef KERNEL_MEM_UTIL_H
#define KERNEL_MEM_UTIL_H

#include <stddef.h>
#include <stdint.h>

void *align_addr_to_page_size_up(void *);
void *align_addr_to_page_size_down(void *);

size_t align_size_to_page_size_up(size_t);
size_t align_size_to_page_size_down(size_t);
  
size_t min_upper_power_of_2(size_t);
size_t max_lower_power_of_2(size_t);

size_t log2_upper(size_t);
size_t log2_lower(size_t);

size_t pages_count(void *begin, void *end);

#endif // #ifndef KERNEL_MEM_UTIL_H
