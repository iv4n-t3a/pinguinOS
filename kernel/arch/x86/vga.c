#include <stdint.h>

#include "core_lib/vga.h"
#include "kernel/arch/x86/bochs.h"
#include "kernel/arch/x86/config.h"
#include "kernel/arch/x86/io.h"

typedef enum {
  VGA_UPDATE_CURSOR_LOW = 0x0F,
  VGA_UPDATE_CURSOR_HIGH = 0x0E,
} vga_command_t;

void kpchar(char c) {
  terminal_putchar(c);

  uint16_t pos =
      terminal_get_row() * terminal_get_width() + terminal_get_column();

  // TODO: Update cursor on end of kprintf only
  port_out_8(k_vga_cursor_port, VGA_UPDATE_CURSOR_LOW);
  port_out_8(k_vga_cursor_port + 1, pos & 0xFF);
  port_out_8(k_vga_cursor_port, VGA_UPDATE_CURSOR_HIGH);
  port_out_8(k_vga_cursor_port + 1, (pos >> 8) & 0xFF);
}

// Use e9 port hack for debug logs
// TODO(not necessary): kernel logs to spi
void dpchar(char c) { bochs_e9_pchar(c); }
