#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
} LogLevel;

/**
 * @brief Logs a message with the specified log level.
 *
 * @param level The log level (INFO, WARN, ERROR).
 * @param fmt The format string for the log message.
 * @param ... Additional arguments for the format string.
 */
void log_message(LogLevel level, const char *fmt, ...);

#endif // LOG_H
