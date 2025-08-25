#include "kernel/arch/x86/idt.h"

#include <stddef.h>
#include <stdint.h>

#include "kernel/libs/stdio.h"

typedef struct {
  uint16_t offset_0_to_16;
  uint16_t segment_selector;
  uint8_t reserved;
  gate_type_t gate_type : 4;
  bool zero : 1;
  uint8_t dpl : 2;
  bool present_bit : 1;
  uint16_t offset_16_to_32;
} PACKED gate_descriptor_t;

typedef struct {
  uint16_t size;
  uint32_t offset;
} PACKED idt_descriptor_t;

#define GATE_FLAGS_PRESENT_BIT (1 << 15)

#define IDT_SIZE 255

gate_descriptor_t idt[IDT_SIZE];

idt_descriptor_t idt_descriptor;

void idt_entry_init(interrupt_gate_t gate) {
  idt[gate.interrupt].offset_0_to_16 = (uint16_t)gate.offset;
  idt[gate.interrupt].segment_selector = gate.segment;
  idt[gate.interrupt].reserved = 0;
  idt[gate.interrupt].gate_type = gate.type;
  idt[gate.interrupt].zero = 0;
  idt[gate.interrupt].dpl = gate.dpl;
  idt[gate.interrupt].present_bit = 1;
  idt[gate.interrupt].offset_16_to_32 = (uint32_t)gate.offset >> 16;
}

void IDT_load(interrupt_gate_t *gates, size_t count,
              interrupt_gate_t default_gate) {
  for (size_t i = 0; i < IDT_SIZE; ++i) {
    default_gate.interrupt = i;
    idt_entry_init(default_gate);
  }

  for (size_t i = 0; i < count; ++i) {
    idt_entry_init(gates[i]);
  }

  idt_descriptor.offset = (uint32_t)idt;
  idt_descriptor.size = sizeof(idt) - 1;
  asm("lidt %0" : : "m"(idt_descriptor));
}
