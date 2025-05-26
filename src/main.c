#include <limits.h>
#include <unistd.h>

#include "../include/init.h"
#include "../include/log.h"

#define PROJECT_NAME "my_project"

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

int main(int argc, char *argv[]) {
  int result = 0;
  char *projectName = PROJECT_NAME;

  if (argc > 1) {
    if (argv[1][0] == '-') {
      if (strcmp(argv[1], "--help") == 0) {
        /* code for help */
      } else if (strcmp(argv[1], "--config") == 0) {
        if (argc <= 2) {
          log_message(
              LOG_ERROR,
              "--config as first argument but no config file path provided");
          result = 1;
          goto cleanup_main;
        }
        log_message(LOG_INFO, "Selected config : %s", argv[2]);
      }
    } else {
      projectName = argv[1];
      char cwd[PATH_MAX];
      if (strchr(projectName, '/')) {
        // TODO: Handle case where projectName contains a path
      } else {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          log_message(LOG_INFO, "Current directory at startup: %s", cwd);
        } else {
          log_message(LOG_ERROR, "getcwd() error");
          result = 1;
          goto cleanup_main;
        }
      }
      if (init_default_setup(cwd, projectName) != 0) {
        log_message(LOG_ERROR, "Failed to initialize default setup.");
        result = 1;
        goto cleanup_main;
      }
    }
  } else {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      log_message(LOG_INFO, "Current directory at startup: %s", cwd);
    } else {
      log_message(LOG_ERROR, "getcwd() error");
      result = 1;
      goto cleanup_main;
    }

    if (init_default_setup(cwd, "my_project") != 0) {
      log_message(LOG_ERROR, "Failed to initialize default setup.");
      result = 1;
      goto cleanup_main;
    }
  }

  log_message(LOG_INFO, "Hello, World!");

cleanup_main:
  if (projectName) free(projectName);
  return result;
}
