#include "kernel/arch/x86/pic.h"

#include <stdint.h>

#include "kernel/arch/x86/io.h"
#include "kernel/libs/kernel_log.h"
#include "kernel/libs/kernel_panic.h"

void PIC_init(uint8_t offset_master, uint8_t offset_slave) {
  // ICW - Initialization control word

  // ICW1: Begin initialization
  port_out_8_and_wait(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
  port_out_8_and_wait(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);

  // ICW2: Tell pics their offsets
  port_out_8_and_wait(MASTER_PIC_DATA, offset_master);
  port_out_8_and_wait(SLAVE_PIC_DATA, offset_slave);

  // ICW3: Tell pics about each other
  port_out_8_and_wait(MASTER_PIC_DATA, 4); // Slave on irq2 (0000 0100)
  port_out_8_and_wait(SLAVE_PIC_DATA, 2);  // Cascade identity (0000 0010)

  // ICW4: 8086 mode
  port_out_8_and_wait(MASTER_PIC_DATA, ICW4_8086);
  port_out_8_and_wait(SLAVE_PIC_DATA, ICW4_8086);

  // Unmask both pics
  port_out_8(MASTER_PIC_DATA, 0);
  port_out_8(SLAVE_PIC_DATA, 0);
}

void PIC_mask_irq(uint16_t mask) {
  port_out_8(MASTER_PIC_DATA, mask & 0xFF);
  port_out_8(SLAVE_PIC_DATA, (mask >> 8) & 0xFF);
}

void PIC_disable() {
  // Mask each interrupt
  port_out_8(MASTER_PIC_DATA, 0xFF);
  port_out_8(SLAVE_PIC_DATA, 0xFF);
}

uint16_t PIC_get_irq_register(int ocw3) {
  port_out_8_and_wait(MASTER_PIC_COMMAND, ocw3);
  port_out_8_and_wait(SLAVE_PIC_COMMAND, ocw3);
  return (port_in_8(SLAVE_PIC_COMMAND) << 16) | port_in_8(MASTER_PIC_COMMAND);
}

int PIC_get_irq_number(int ocw3) {
  uint16_t irq_reg = PIC_get_irq_register(ocw3);
  irq_reg &= ~(1 << 2); // Unset irq2, as it always set

  // Do some bitwise magic to obtain the only set bit
  uint32_t result = 0;

  while ((1 << result) != irq_reg) {
    result += 1;

    if (result == 0) {
      log_fatal("Can't find irq for irq reg %d", irq_reg);
      kernel_panic();
    }
  }

  return result;
}

uint8_t PIC_get_processing_irq() {
  return PIC_get_irq_number(PIC_CMD_READ_ISR);
}

uint8_t PIC_get_pending_irq() { return PIC_get_irq_number(PIC_CMD_READ_IRR); }

void PIC_send_end_of_interrupt() {
  uint8_t irq = PIC_get_processing_irq();

  if (irq > 8) {
    port_out_8_and_wait(SLAVE_PIC_COMMAND, PIC_CMD_EOI);
  }
  port_out_8_and_wait(MASTER_PIC_COMMAND, PIC_CMD_EOI);
}
