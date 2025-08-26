#ifndef KERNEL_ARCH_X86_IO_H
#define KERNEL_ARCH_X86_IO_H

#include <stdint.h>

typedef uint16_t port_t;

static inline uint8_t port_in_8(port_t port) {
  uint8_t res;
  asm volatile("inb %%dx, %%al" : "=a"(res) : "d"(port));
  return res;
}

static inline uint16_t port_in_16(port_t port) {
  uint16_t res;
  asm volatile("inw %%dx, %%ax" : "=a"(res) : "d"(port));
  return res;
}

static inline uint32_t port_in_32(port_t port) {
  uint32_t res;
  asm volatile("inl %%dx, %%eax" : "=a"(res) : "d"(port));
  return res;
}

static inline void port_out_8(port_t port, uint8_t data) {
  asm volatile("outb %%al, %%dx" : : "a"(data), "d"(port));
}

static inline void port_out_16(port_t port, uint16_t data) {
  asm volatile("outw %%ax, %%dx" : : "a"(data), "d"(port));
}

static inline void port_out_32(port_t port, uint32_t data) {
  asm volatile("outl %%eax, %%dx" : : "a"(data), "d"(port));
}

static inline void io_wait() {
  // Out to unused port to wait small amount of time

  // May look like not propper way of doing delay, but it's
  // very common way to wait io operations in os dev.

  port_out_8(0x80, 0);
}

static inline void port_out_8_and_wait(port_t port, uint8_t data) {
  port_out_8(port, data);
  io_wait();
}

static inline void port_out_16_and_wait(port_t port, uint16_t data) {
  port_out_16(port, data);
  io_wait();
}

static inline void port_out_32_and_wait(port_t port, uint32_t data) {
  port_out_32(port, data);
  io_wait();
}

#endif // #ifndef KERNEL_ARCH_X86_IO_H
