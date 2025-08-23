#include "boot/stage2/error.h"

#include "core_lib/vga.h"

void error_log(error_t error) {
  terminal_writestring("STAGE2: ");

  switch (error) {
  case ERR_SUCCESS:
    terminal_writestring("Success error shouldn't be logged.\n");
    break;
  case ERR_DAPACK_INIT_ERROR:
    terminal_writestring(
        "Failed to init disk address pack, refactore memory map.\n");
    break;
  case ERR_BIOS_CALL_ERROR:
    terminal_writestring("Bios call error.\n");
    break;
  case ERR_PARTITION_NOT_FOUND:
    terminal_writestring("Can't find valid pinguin OS partition.\n");
    break;
  case ERR_SIGNATURE_VERIFICATION_FAILED:
    terminal_writestring("Signature verification error.\n");
    break;
  case ERR_INVALID_FILE_NAME:
    terminal_writestring("Invalid filename error.\n");
    break;
  case ERR_FILE_NOT_FOUND:
    terminal_writestring("File not found error.\n");
    break;
  case ERR_INCOMPATIBLE_BUFFER_SIZE:
    terminal_writestring("Incompatible buffer size error.\n");
    break;
  case ERR_INCOMPATIBLE_ELF:
    terminal_writestring("Incompatible elf file error.\n");
    break;
  case ERR_UNIMPLEMENTED:
    terminal_writestring("Unimplemented hit.\n");
    break;
  case ERR_KERNEL_EXITED:
    terminal_writestring("Error. Run kernel elf shall never return.\n");
    break;
  default:
    terminal_writestring("Unknown error.\n");
    break;
  }
}
