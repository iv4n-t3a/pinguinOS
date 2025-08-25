#ifndef KERNEL_LIBS_KERNEL_LOG_H
#define KERNEL_LIBS_KERNEL_LOG_H

// TODO: Add log level separation, filtering e.g.

void log_debug(const char *fmt, ...);

void log_info(const char *fmt, ...);

void log_warning(const char *fmt, ...);

void log_error(const char *fmt, ...);

void log_fatal(const char *fmt, ...);

#endif // #ifndef KERNEL_LIBS_KERNEL_LOG
