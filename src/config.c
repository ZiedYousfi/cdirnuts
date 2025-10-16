#include "../include/config.h"

int executeCommand(const char *command) {
  if (command == NULL) {
    log_error("Command is NULL");
    return -1;
  }

  printf("About to execute command: %s\n", command);
  printf("Are you sure? (y/n): ");
  fflush(stdout);
  char confirmation = getchar();

  if (confirmation == EOF) {
    log_error("Failed to read confirmation");
    return -1;
  }

  if (confirmation != 'y' && confirmation != 'Y') {
    log_info("Command '%s' not confirmed", command);
    return -1;
  }

  int result = system(command);
  if (result == -1) {
    log_error("Failed to execute command: %s", command);
    return -1;
  }

#ifdef _WIN32
  int exitStatus = result;
  if (exitStatus != 0) {
    log_error("Command '%s' exited with status %d", command, exitStatus);
    return exitStatus;
  }
#else
  if (WIFEXITED(result)) {
    int exitStatus = WEXITSTATUS(result);
    if (exitStatus != 0) {
      log_error("Command '%s' exited with status %d", command, exitStatus);
      return exitStatus;
    }
  } else {
    log_error("Command '%s' did not terminate normally", command);
    return -1;
  }
#endif

  return 0;
}
