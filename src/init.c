#include "../include/init.h"

int init_default_setup(const char *parentDir, const char *projectName) {
  int result = 0;
  FILE *mainFile = NULL;
  if (parentDir == NULL || projectName == NULL) {
    log_message(LOG_ERROR, "Parent directory or project name cannot be NULL.");
    return -1;
  }
  if (strlen(projectName) == 0) {
    log_message(LOG_ERROR, "Project name cannot be an empty string.");
    return -1;
  }
  if (strlen(parentDir) == 0) {
    log_message(LOG_ERROR, "Parent directory cannot be an empty string.");
    return -1;
  }

  if (createDir(projectName, parentDir)) {
    log_message(LOG_ERROR, "Failed to create project directory.");
    return -1;
  }

  char *projectDir = constructPath(projectName, parentDir);
  if (projectDir == NULL) {
    log_message(LOG_ERROR, "Failed to construct the project directory path.");
    result = -1;
    goto cleanup;
  }

  log_message(LOG_INFO, "Project directory '%s' created successfully in '%s'.",
              projectName, parentDir);

  log_message(LOG_INFO, "Project directory path: %s", projectDir);

  if (createDir("include", projectDir)) {
    log_message(LOG_ERROR, "Failed to create include directory.");
    result = -1;
    goto cleanup;
  }

  if (createDir("src", projectDir)) {
    log_message(LOG_ERROR, "Failed to create src directory.");
    result = -1;
    goto cleanup;
  }
  char *srcPath = constructPath("src", projectDir);

  FILE *mainFile = createFile("main.c", srcPath);
  if (mainFile == NULL) {
    log_message(LOG_ERROR, "Failed to create main.c file.");
    result = -1;
    goto cleanup;
  }

cleanup:
  fclose(mainFile);
  free(srcPath);
  free(projectDir);
  return result;
}
