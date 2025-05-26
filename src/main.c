#include <limits.h>
#include <unistd.h>

#include "../include/init.h"
#include "../include/log.h"

#define PROJECT_NAME "my_project"

int main(int argc, char *argv[]) {
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
          return -1;
        }
        log_message(LOG_INFO, "Selected config : %s", argv[2]);
      }
    } else {
      projectName = argv[1];
      char cwd[PATH_MAX];
      if (strchr(projectName, '/')) {
        // TODO: handle path in arg
      } else {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          log_message(LOG_INFO, "Current directory at startup: %s", cwd);
        } else {
          log_message(LOG_ERROR, "getcwd() error");
          return 1;
        }
      }
      if (init_default_setup(cwd, projectName) != 0) {
        log_message(LOG_ERROR, "Failed to initialize default setup.");
        return 1;
      }
    }
  } else {
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
  }

  log_message(LOG_INFO, "Hello, World!");

  // Return 0 to indicate successful execution
  return 0;
}
