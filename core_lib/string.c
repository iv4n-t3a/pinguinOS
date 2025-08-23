#include "core_lib/string.h"
#include "core_lib/vga.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

void itos(uint32_t num, char *str) {
  if (num == 0) {
    str[0] = '0';
    str[1] = '\0';
    return;
  }

  uint32_t denum = 1;

  while (num / denum >= 10) {
    denum *= 10;
  }

  size_t i = 0;
  while (denum != 0) {
    str[i] = num / denum % 10 + '0';
    denum /= 10;
    i += 1;
  }
  str[i] = '\0';
}

bool strcmp(const char *str1, const char *str2, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    if (str1[i] != str2[i]) {
      return false;
    }
    if (str1[i] == '\0') {
      return true;
    }
  }
  return true;
}

void *memcpy(char *dst, const char *src, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    dst[i] = src[i];
  }
  return dst;
}

void *memset(void *mem, int val, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    *(char *)(mem + i) = val;
  }
  return mem;
}
