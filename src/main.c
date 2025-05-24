#include <limits.h>
#include <unistd.h>

#include "../include/log.h"
#include "../include/init.h"

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

  if (init_default_setup(cwd, "my_project") != 0) {
    log_message(LOG_ERROR, "Failed to initialize default setup.");
    return 1;
  }

  log_message(LOG_INFO, "Hello, World!");

  // Return 0 to indicate successful execution
  return 0;
}
