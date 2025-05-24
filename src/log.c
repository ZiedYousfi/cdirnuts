#include "../include/log.h"

#include <stdarg.h>

/**
 * @brief Logs a message with the specified log level.
 *
 * @param level The log level (INFO, WARN, ERROR).
 * @param fmt The format string for the log message.
 * @param ... Additional arguments for the format string.
 */
void log_message(LogLevel level, const char *fmt, ...) {
  const char *level_str = "";
  switch (level) {
    case LOG_INFO:
      level_str = "INFO";
      break;
    case LOG_WARN:
      level_str = "WARN";
      break;
    case LOG_ERROR:
      level_str = "ERROR";
      break;
  }
  fprintf(stderr, "[%s] ", level_str);
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
}
