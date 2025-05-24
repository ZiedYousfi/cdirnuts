#include <limits.h>
#include <unistd.h>

#include "../include/dir_utils.h"
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
