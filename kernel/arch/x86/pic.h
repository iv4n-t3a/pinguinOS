#ifndef KERNEL_ARCH_X86_PIC_H
#define KERNEL_ARCH_X86_PIC_H

#include "kernel/arch/x86/io.h"

#include <stdint.h>

typedef enum {
  MASTER_PIC_COMMAND = 0x20,
  MASTER_PIC_DATA = 0x21,
  SLAVE_PIC_COMMAND = 0xA0,
  SLAVE_PIC_DATA = 0xA1
} pic_port_t;

typedef enum {
  ICW1_ICW4 = 0x01,      // Indicates that ICW4 will be present
  ICW1_SINGLE = 0x02,    // Single (cascade) mode
  ICW1_INTERVAL4 = 0x04, // Call address interval 4 (8)
  ICW1_LEVEL = 0x08,     // Level triggered (edge) mode
  ICW1_INIT = 0x10       // Initialization - required!
} pic_icw1_t;

typedef enum {
  ICW4_8086 = 0x01,       // 8086/88 (MCS-80/85) mode
  ICW4_AUTO = 0x02,       // Auto (normal) EOI
  ICW4_BUF_SLAVE = 0x08,  // Buffered mode/slave
  ICW4_BUF_MASTER = 0x0C, // Buffered mode/master
  ICW4_SFNM = 0x10        // Special fully nested (not)
} pic_icw4_t;

typedef enum {
  PIC_CMD_INIT = 0x11,
  PIC_CMD_EOI = 0x20,
  PIC_CMD_READ_IRR = 0x0A,
  PIC_CMD_READ_ISR = 0x0B
} pic_cmd_t;

void PIC_init(uint8_t offset_master, uint8_t offset_slave);

void PIC_mask_irq(uint16_t mask);

void PIC_disable();

uint8_t PIC_get_processing_irq();

uint8_t PIC_get_pending_irq();

void PIC_send_end_of_interrupt();

#endif // #ifndef KERNEL_ARCH_X86_PIC_H
