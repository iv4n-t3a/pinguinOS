#include "boot/stage2/error.h"

#include "boot/stage2/stdio.h"
#include "error.h"

void trace_error(error_t error, const char *file, const char *func, int line) {
  if (!error) {
    return;
  }
  printf("Error trace: %s:%s:%d\n", file, func, line);
}

void error_log(error_t error) {
  putstr("STAGE2 error: ");

  switch (error) {
  case ERR_SUCCESS:
    putstr("Success error shouldn't be logged.\n");
    break;
  case ERR_DAPACK_INIT_ERROR:
    putstr(
        "Failed to init disk address pack, refactore memory map.\n");
    break;
  case ERR_BIOS_CALL_ERROR:
    putstr("Bios call error.\n");
    break;
  case ERR_PARTITION_NOT_FOUND:
    putstr("Can't find valid pinguin OS partition.\n");
    break;
  case ERR_SIGNATURE_VERIFICATION_FAILED:
    putstr("Signature verification error.\n");
    break;
  case ERR_INVALID_FILE_NAME:
    putstr("Invalid filename error.\n");
    break;
  case ERR_FILE_NOT_FOUND:
    putstr("File not found error.\n");
    break;
  case ERR_INCOMPATIBLE_BUFFER_SIZE:
    putstr("Incompatible buffer size error.\n");
    break;
  case ERR_INCOMPATIBLE_ELF:
    putstr("Incompatible elf file error.\n");
    break;
  case ERR_UNIMPLEMENTED:
    putstr("Unimplemented hit.\n");
    break;
  case ERR_KERNEL_EXITED:
    putstr("Error. Run kernel elf shall never return.\n");
    break;
  case ERR_TERMINAL_CLUSTER_READ_ATTEMPT:
    putstr("Error. Attempt to read terminal FAT cluster.\n");
    break;
  case ERR_BUFFER_OVERFLOW:
    putstr("Error. Buffer overflow.\n");
    break;
  default:
    putstr("Unknown error.\n");
    break;
  }
}
