#include "kernel/arch/x86/gdt.h"

#include <stdint.h>

#include "kernel/compiler.h"

typedef struct {
  uint16_t limit_0_to_16;
  uint16_t base_0_to_16;
  uint8_t base_16_to_24;
  uint8_t access_byte;
  uint8_t limit_16_to_20 : 4;
  uint8_t flags : 4;
  uint8_t base_24_to_32;
} PACKED GDT_segment_descriptor_t;

typedef struct {
  uint16_t size;
  uint32_t offset;
} PACKED GDT_descriptor_t;

#define GDT_DESCRIPTOR(base, limit, flags, access_byte)                        \
  {limit, base, base >> 16, access_byte, flags, limit >> 16, base >> 24}

#define GDT_FLAGS_GRANULARITY 8
#define GDT_FLAGS_SIZE 4
#define GDT_FLAGS_LONG_MODE 1

#define GDT_AB_PRESENT_BIT 0x80
#define GDT_AB_DPL0 0x00
#define GDT_AB_DPL1 0x20
#define GDT_AB_DPL2 0x40
#define GDT_AB_DPL3 0x60
#define GDT_AB_TYPE_BIT 0x10 // If set segment is code. System seg otherwise
#define GDT_AB_EXECUTABLE 0x08
#define GDT_AB_CONFORMING_BIT                                                  \
  0x04 // Set if can be executed from an equal or lower dpl
#define GDT_AB_DIRECTION_BIT 0x04 // Set if data segment grows down
#define GDT_AB_CODE_READABLE 0x02
#define GDT_AB_DATA_WRITABLE 0x02
#define GDT_AB_ACCESS_BIT 0x02

GDT_segment_descriptor_t gdt[] = {
    GDT_DESCRIPTOR(0, 0, 0, 0),
    GDT_DESCRIPTOR(0, 0xFFFFFFFF, GDT_FLAGS_GRANULARITY | GDT_FLAGS_SIZE,
                   GDT_AB_PRESENT_BIT | GDT_AB_TYPE_BIT | GDT_AB_EXECUTABLE |
                       GDT_AB_CODE_READABLE),
    GDT_DESCRIPTOR(0, 0xFFFFFFFF, GDT_FLAGS_GRANULARITY | GDT_FLAGS_SIZE,
                   GDT_AB_PRESENT_BIT | GDT_AB_TYPE_BIT | GDT_AB_DATA_WRITABLE),
};

GDT_descriptor_t gdt_descriptor;

void GDT_load() {
  gdt_descriptor.offset = (uint32_t)gdt;
  gdt_descriptor.size = sizeof(gdt);
  asm("lgdt %0" : : "m"(gdt_descriptor));
  asm("ljmp $1 << 3, $1f \n1:");
}
