#include "../include/log.h"
#include "../include/path_utils.h"

char *copySubstring(const char *source, int start, int end) {
  if (source == NULL || start < 0 || end < start || end >= (int)strlen(source)) {
    log_message(LOG_ERROR, "Invalid parameters for substring copy.");
    return NULL;
  }
  int length = end - start + 1;
  char *substring = malloc(length + 1);
  if (substring == NULL) {
    log_message(LOG_ERROR, "Memory allocation failed for substring.");
    return NULL;
  }
  strncpy(substring, source + start, length);
  substring[length] = '\0';
  return substring;
}

char *constructPath(const char *dirName, const char *parentDir) {
  if (parentDir == NULL) {
    log_message(LOG_ERROR, "Parent directory cannot be NULL.");
    return NULL;
  }
  if (dirName == NULL) {
    log_message(LOG_ERROR, "Directory name cannot be NULL.");
    return NULL;
  }
  char fullPath[PATH_MAX];
  if (snprintf(fullPath, sizeof(fullPath), "%s/%s", parentDir, dirName) >=
      (int)sizeof(fullPath)) {
    log_message(LOG_ERROR, "Full path is too long.");
    return NULL;
  }
  return strdup(fullPath);
}
