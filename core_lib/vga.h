#ifndef CORE_LIB_VGA_H
#define CORE_LIB_VGA_H

// This code is stolen from https://wiki.osdev.org/Bare_Bones

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core_lib/string.h"

enum VgaColor {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum VgaColor fg, enum VgaColor bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

void terminal_initialize(size_t width, size_t height);

void terminal_move_cursor(int x, int y);

void terminal_clear();

void terminal_setcolor(uint8_t color);

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

void terminal_putchar(char c);

void terminal_line_break();

void terminal_write(const char *data, size_t size);

void terminal_writestring(const char *data);

void terminal_writeint(uint32_t num);

#endif // #ifndef CORE_LIB_VGA_H
