#include "../include/init.h"

int init_default_setup(const char *parentDir, const char *projectName) {
  int result = 0;
  FILE *mainFile = NULL;
  char *projectDir = NULL;
  char *srcPath = NULL;

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

  projectDir = constructPath(projectName, parentDir);
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

  srcPath = constructPath("src", projectDir);
  if (srcPath == NULL) {
    log_message(LOG_ERROR, "Failed to construct src directory path.");
    result = -1;
    goto cleanup;
  }

  mainFile = createFile("main.c", srcPath);
  if (mainFile == NULL) {
    log_message(LOG_ERROR, "Failed to create main.c file.");
    result = -1;
    goto cleanup;
  }

cleanup:
  if (mainFile) fclose(mainFile);
  if (srcPath) free(srcPath);
  if (projectDir) free(projectDir);
  return result;
}
