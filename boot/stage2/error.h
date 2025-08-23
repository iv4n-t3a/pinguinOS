#ifndef BOOT_STAGE2_ERROR_H
#define BOOT_STAGE2_ERROR_H

typedef enum {
  ERR_SUCCESS,
  ERR_DAPACK_INIT_ERROR,
  ERR_BIOS_CALL_ERROR,
  ERR_PARTITION_NOT_FOUND,
  ERR_SIGNATURE_VERIFICATION_FAILED,
  ERR_INVALID_FILE_NAME,
  ERR_FILE_NOT_FOUND,
  ERR_INCOMPATIBLE_BUFFER_SIZE,
  ERR_INCOMPATIBLE_ELF,
  ERR_UNIMPLEMENTED,
  ERR_KERNEL_EXITED,
  ERR_TERMINAL_CLUSTER_READ_ATTEMPT,
} error_t;

#define NEW_ERR(err) trace_error(err, __FILE__, __func__, __LINE__), err

#define ERR_HANDLE_MAIN(func_call)                                             \
  if (error_t err = func_call) {                                               \
    error_log(err);                                                            \
    for (;;) {                                                                 \
    };                                                                         \
  }

#define ERR_HANDLE_SUBROUTINE(func_call)                                       \
  if (error_t err = func_call) {                                               \
    trace_error(err, __FILE__, __func__, __LINE__);                            \
    return err;                                                                \
  }

void trace_error(error_t error, const char *file, const char *func, int line);

void error_log(error_t error);

#endif // #ifndef BOOT_STAGE2_ERROR_H
