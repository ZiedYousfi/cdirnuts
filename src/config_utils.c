#include "../include/config_utils.h"

int executeCommand(const char *command) {
  if (command == NULL) {
    log_message(LOG_ERROR, "Command is NULL");
    return -1;
  }

  int result = system(command);
  if (result == -1) {
    log_message(LOG_ERROR, "Failed to execute command: %s", command);
    return -1;
  }

  if (WIFEXITED(result)) {
    int exitStatus = WEXITSTATUS(result);
    if (exitStatus != 0) {
      log_message(LOG_ERROR, "Command '%s' exited with status %d", command,
                  exitStatus);
      return exitStatus;
    }
  } else {
    log_message(LOG_ERROR, "Command '%s' did not terminate normally", command);
    return -1;
  }

  return 0;
}
