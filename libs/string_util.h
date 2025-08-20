#ifndef LIB_STRING_UTIL_H
#define LIB_STRING_UTIL_H

#include <stddef.h>

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

#endif  // #ifndef LIB_STRING_UTIL_H
