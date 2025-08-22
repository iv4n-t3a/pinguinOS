// This code is stolen from https://wiki.osdev.org/Bare_Bones

#include "core_lib/vga.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core_lib/string.h"

size_t terminal_width;
size_t terminal_height;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize(size_t width, size_t height) {
  terminal_width = width;
  terminal_height = height;
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t *)0xB8000;
  for (size_t y = 0; y < terminal_height; y++) {
    for (size_t x = 0; x < terminal_width; x++) {
      const size_t index = y * terminal_width + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * terminal_width + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  if (c == '\0') {
    return;
  }

  if (c == '\r') {
    terminal_column = 0;
    return;
  }

  if (c == '\n') {
    ++terminal_row;
    terminal_row %= terminal_height;
    terminal_column = 0;
    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == terminal_width) {
    terminal_column = 0;
    if (++terminal_row == terminal_height)
      terminal_row = 0;
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

void terminal_writeint(uint32_t num) {
  char str[9];
  itos(num, str);
  terminal_putchar('0');
  terminal_putchar('x');
  terminal_write(str, 9);
}
