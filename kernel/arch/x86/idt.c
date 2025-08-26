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

void idt_entry_init(idtentry_t entry) {
  idt[entry.interrupt].offset_0_to_16 = (uint16_t)entry.offset;
  idt[entry.interrupt].segment_selector = entry.segment;
  idt[entry.interrupt].reserved = 0;
  idt[entry.interrupt].gate_type = entry.type;
  idt[entry.interrupt].zero = 0;
  idt[entry.interrupt].dpl = entry.dpl;
  idt[entry.interrupt].present_bit = entry.present_bit;
  idt[entry.interrupt].offset_16_to_32 = (uint32_t)entry.offset >> 16;
}

void IDT_load(idtentry_t *entries, size_t count) {
  idtentry_t default_entry = { 0 };

  for (size_t i = 0; i < IDT_SIZE; ++i) {
    default_entry.interrupt = i;
    idt_entry_init(default_entry);
  }

  for (size_t i = 0; i < count; ++i) {
    idt_entry_init(entries[i]);
  }

  idt_descriptor.offset = (uint32_t)idt;
  idt_descriptor.size = sizeof(idt) - 1;
  asm("lidt %0" : : "m"(idt_descriptor));
  asm("sti");
}
