#include "kernel_libs/string_util.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

void itos(uint32_t num, char* str) {
  for (size_t i = 0; i < 8; ++i) {
    char digit = (num >> (28 - i * 4)) & 0xF;

    if (digit <= 9) {
      str[i] = digit + '0';
    } else if (digit <= 15) {
      str[i] = digit + 'A' - 10;
    } else {
      str[i] = 'n';
    }
  }
  str[8] = '\0';
}

bool strcmp(const char* str1, const char* str2, size_t size) {
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
