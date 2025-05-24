#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
} LogLevel;

void log_message(LogLevel level, const char *fmt, ...);

#endif // LOG_H
