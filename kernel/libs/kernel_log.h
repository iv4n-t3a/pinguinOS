#ifndef KERNEL_LIBS_KERNEL_LOG_H
#define KERNEL_LIBS_KERNEL_LOG_H

#include "kernel/config.h"

// Sadly C doesn't allow using define inside macro definition. Since there isn't
// a lot of log levels I found copy-past solution more appropriate than code
// generation

#define LOG_INTERNAL(level, fmt, ...)                                          \
  log_##level(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INTERNAL_APPEND(level, fmt, ...)                                   \
  log_##level##_append(fmt, ##__VA_ARGS__)

#define DECLARE_LOG_FUNC(level)                                                \
  void log_##level(const char *file, const char *func, int line,               \
                   const char *fmt, ...);                                      \
                                                                               \
  void log_##level##_append(const char *fmt, ...);

#if USE_DEBUG_LOGS && ENABLE_LOGS
#define LOG_DEBUG(fmt, ...) LOG_INTERNAL(DEBUG, fmt, ##__VA_ARGS__)
#define LOG_DEBUG_APPEND(fmt, ...)                                             \
  LOG_INTERNAL_APPEND(DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#define LOG_DEBUG_APPEND(fmt, ...)
#endif

#if USE_INFO_LOGS && ENABLE_LOGS
#define LOG_INFO(fmt, ...) LOG_INTERNAL(INFO, fmt, ##__VA_ARGS__)
#define LOG_INFO_APPEND(fmt, ...) LOG_INTERNAL_APPEND(INFO, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#define LOG_INFO_APPEND(fmt, ...)
#endif

#if USE_WARNING_LOGS && ENABLE_LOGS
#define LOG_WARNING(fmt, ...) LOG_INTERNAL(WARNING, fmt, ##__VA_ARGS__)
#define LOG_WARNING_APPEND(fmt, ...)                                           \
  LOG_INTERNAL_APPEND(WARNING, fmt, ##__VA_ARGS__)
#else
#define LOG_WARNING(fmt, ...)
#define LOG_WARNING_APPEND(fmt, ...)
#endif

#if USE_ERROR_LOGS && ENABLE_LOGS
#define LOG_ERROR(fmt, ...) LOG_INTERNAL(ERROR, fmt, ##__VA_ARGS__)
#define LOG_ERROR_APPEND(fmt, ...)                                             \
  LOG_INTERNAL_APPEND(ERROR, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#define LOG_ERROR_APPEND(fmt, ...)
#endif

#if USE_FATAL_LOGS && ENABLE_LOGS
#define LOG_FATAL(fmt, ...) LOG_INTERNAL(FATAL, fmt, ##__VA_ARGS__)
#define LOG_FATAL_APPEND(fmt, ...)                                             \
  LOG_INTERNAL_APPEND(FATAL, fmt, ##__VA_ARGS__)
#else
#define LOG_FATAL(fmt, ...)
#define LOG_FATAL_APPEND(fmt, ...)
#endif

DECLARE_LOG_FUNC(DEBUG);
DECLARE_LOG_FUNC(INFO);
DECLARE_LOG_FUNC(WARNING);
DECLARE_LOG_FUNC(ERROR);
DECLARE_LOG_FUNC(FATAL);

#endif // #ifndef KERNEL_LIBS_KERNEL_LOG
