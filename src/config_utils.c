#include "../include/config_utils.h"

int executeCommand(const char *command) {
    if (command == NULL) {
        logError("Command is NULL");
        return -1;
    }

    int result = system(command);
    if (result == -1) {
        logError("Failed to execute command: %s", command);
        return -1;
    }

    if (WIFEXITED(result)) {
        int exitStatus = WEXITSTATUS(result);
        if (exitStatus != 0) {
            logError("Command '%s' exited with status %d", command, exitStatus);
            return exitStatus;
        }
    } else {
        logError("Command '%s' did not terminate normally", command);
        return -1;
    }

    return 0;
}
