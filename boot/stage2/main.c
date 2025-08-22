#include <stdbool.h>
#include <stdint.h>

#include "boot/stage2/error_log.h"
#include "boot/stage2/find_partition.h"
#include "boot/stage2/load_file.h"
#include "boot/stage2/load_sectors.h"
#include "core_lib/elf.h"
#include "core_lib/mbr.h"
#include "core_lib/string.h"
#include "core_lib/vga.h"

void __attribute__((cdecl)) bootmain(int boot_drive) {
  void *kernel_address = (void *)0x100000;

  terminal_initialize(80, 25);

  MBR_partition_table_t *pt;
  int partition;

  int err = find_partition(boot_drive, &pt, &partition);

  if (err != INIT_FAT_SUCCESS) {
    error_log("Error at searching pinguinOS partition", err);
    return;
  }

  err = init_fat(boot_drive, pt->entries + partition);

  if (err != INIT_FAT_SUCCESS) {
    error_log("Error at fat initialization", err);
    return;
  }

  err = load_file(kernel_address, "KERNEL  ELF");

  if (err != LOAD_FILE_SUCCESS) {
    error_log("Error at loading kernel", err);
    return;
  }

  ELF_header32_t *header = (ELF_header32_t *)kernel_address;

  // TODO: Verify other elf parameters;
  if (header->magic[0] != 0x7F) {
    error_log("Kernel magic ELF not match", 1);
    return;
  }

  void (*kmain)();
  kmain = (void *)header->program_entry_offset;

  terminal_write(kernel_address, 4);
  terminal_putchar('\n');
  terminal_writeint(header->program_entry_offset);

  for (;;) {
  }
}
