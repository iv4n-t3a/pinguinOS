#ifndef CORE_LIB_STRING_H
#define CORE_LIB_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
void itos(uint32_t num, char *str);

bool pos_strcmp(const char *str1, const char *str2, size_t size);
void *pos_memcpy(char *dst, const char *src, size_t count);
void *pos_memset(void *mem, uint8_t val, size_t count);

// Looks like good code (no)
// this is hack to make core_lib compilable with stdlib

#define strcmp pos_strcmp
#define memcpy pos_memcpy
#define memset pos_memset

#endif // #ifndef CORE_LIB_STRING_H
