#include <stdio.h>

void kpchar(char c) {
  putchar(c);
  fflush(stdout);
}

void dpchar(char c) { fputc(c, stderr); }
