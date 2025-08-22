#ifndef LIB_ELF_H
#define LIB_ELF_H

#include <stdint.h>

typedef enum {
  ELF_BITNESS_32 = 1,
  ELF_BITNESS_64 = 2
} elf_bitness_t;

typedef enum {
  ELF_LITTLE_ENDIAN = 1,
  ELF_BIG_ENDIAN = 2
} elf_endianess_t;

typedef enum {
  ELF_TYPE_RELOCATABLE = 1,
  ELF_TYPE_EXECUTABLE = 2,
  ELF_TYPE_SHARED = 3,
  ELF_TYPE_CORE = 3
} elf_type_t;

typedef enum {
  ELF_INSTRUCTION_SET_NO_SPECIFIC = 0x0,
  ELF_INSTRUCTION_SET_SPARC = 0x2,
  ELF_INSTRUCTION_SET_X86 = 0x3,
  ELF_INSTRUCTION_SET_MIPS = 0x8,
  ELF_INSTRUCTION_SET_POWER_PC = 0x14,
  ELF_INSTRUCTION_SET_ARM = 0x28,
  ELF_INSTRUCTION_SET_SUPER_H = 0x2A,
  ELF_INSTRUCTION_SET_IA64 = 0x32,
  ELF_INSTRUCTION_SET_X86_64 = 0x3E,
  ELF_INSTRUCTION_SET_AARCH64 = 0xB7,
  ELF_INSTRUCTION_SET_RISC_V = 0xF3,
} elf_instruction_set_t;

typedef struct {
  uint8_t magic[4]; // 0x7F, then 'ELF' in ASCII
  uint8_t bitness;  // See elf_bitness_t
  uint8_t endianess;  // See elf_endianess_t
  uint8_t elf_header_version;
  uint8_t os_abi;
  uint64_t padding;
  uint16_t type; // See elf_type_t
  uint16_t instruction_set; // See elf_instruction_set_t
  uint32_t elf_version;
  uint32_t program_entry_offset;
  uint32_t program_header_table_offset;
  uint32_t section_header_table_offset;
  uint32_t flags;
  uint16_t elf_header_size;
  uint16_t program_header_table_entry_size;
  uint16_t program_header_table_entries_count;
  uint16_t section_header_table_entry_size;
  uint16_t section_header_table_entries_count;
  uint16_t section_index;
} __attribute__((packed)) ELF_header32_t;

typedef struct {
  uint8_t magic[4];
  uint8_t bitness;  // See elf_bitness_t
  uint8_t endianess;  // See elf_endianess_t
  uint8_t elf_header_version;
  uint8_t os_abi;
  uint64_t padding;
  uint16_t type; // See elf_type_t
  uint16_t instruction_set; // See elf_instruction_set_t
  uint32_t elf_version;
  uint64_t program_entry_offset;
  uint64_t program_header_table_offset;
  uint64_t section_header_table_offset;
  uint32_t flags;
  uint16_t elf_header_size;
  uint16_t program_header_table_entry_size;
  uint16_t program_header_table_entries_count;
  uint16_t section_header_table_entry_size;
  uint16_t section_header_table_entries_count;
  uint16_t section_index;
} __attribute__((packed)) ELF_header64_t;

typedef struct {
  uint32_t segtype;
  uint32_t offset;
  uint32_t vaddr;
  uint32_t paddr;
  uint32_t filesz;
  uint32_t memsz;
  uint32_t flags;
  uint32_t alignment;
} ELF_program_header32_t;

typedef struct {
  uint32_t segtype;
  uint32_t flags;
  uint64_t offset;
  uint64_t vaddr;
  uint64_t paddr;
  uint64_t filesz;
  uint64_t memsz;
  uint64_t alignment;
} ELF_program_header64_t;

#endif // #ifndef LIB_ELF_H
