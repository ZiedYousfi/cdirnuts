#include "../include/log.h"

#include <stdarg.h>

void log_message(LogLevel level, const char* fmt, ...) {
  const char* level_str = "";
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
