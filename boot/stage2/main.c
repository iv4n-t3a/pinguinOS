#include <stdbool.h>
#include <stdint.h>

#include "boot/stage2/error.h"
#include "boot/stage2/fat.h"
#include "boot/stage2/find_partition.h"
#include "boot/stage2/read_disk.h"
#include "boot/stage2/run_elf.h"
#include "core_lib/elf.h"
#include "core_lib/mbr.h"
#include "core_lib/string.h"
#include "core_lib/vga.h"

void __attribute__((cdecl)) bootmain(int boot_drive) {
  void *kernel_address = (void *)0x100000;

  terminal_initialize(80, 25);

  MBR_partition_table_t *pt;
  int partition;

  ERR_HANDLE_MAIN(find_partition(boot_drive, &pt, &partition));
  ERR_HANDLE_MAIN(FAT_init(boot_drive, pt->entries + partition));
  ERR_HANDLE_MAIN(run_elf("KERNEL  ELF", kernel_address));
}
