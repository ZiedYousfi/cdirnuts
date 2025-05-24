#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/log.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    log_message(LOG_INFO, "Current directory at startup: %s", cwd);
  } else {
    log_message(LOG_ERROR, "getcwd() error");
    return 1;
  }

  // Create a directory named "test_dir" in the current working directory
  if (createDir("test_dir", cwd) != 0) {
    log_message(LOG_ERROR, "Failed to create directory.");
    return 1;
  }
  // Log a message to indicate successful execution
  log_message(LOG_INFO, "Directory created successfully.");

  log_message(LOG_INFO, "Hello, World!");

  // Return 0 to indicate successful execution
  return 0;
}

int createDir(const char *dirName, const char *parentDir) {
  if (parentDir == NULL) {
    log_message(LOG_ERROR, "Parent directory cannot be NULL.");
    return -1;
  }
  if (dirName == NULL) {
    log_message(LOG_ERROR, "Directory name cannot be NULL.");
    return -1;
  }
  char fullPath[PATH_MAX];
  if (snprintf(fullPath, sizeof(fullPath), "%s/%s", parentDir, dirName) >=
      (int)sizeof(fullPath)) {
    log_message(LOG_ERROR, "Full path is too long.");
    return -1;
  }
  int statut = mkdir(fullPath, 0755);  // droits d'accès
  if (statut == 0) {
    log_message(LOG_INFO, "Directory %s was successfully created in %s!",
                dirName, parentDir);
  } else {
    log_message(LOG_ERROR, "Error creating directory");
  }

  return 0;
}
