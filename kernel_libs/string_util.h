#ifndef LIB_STRING_UTIL_H
#define LIB_STRING_UTIL_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);

void itos(uint32_t num, char* str);

bool strcmp(const char* str1, const char* str2, size_t size);

void *memcpy(char *dst, const char *src, size_t count);

#endif  // #ifndef LIB_STRING_UTIL_H
