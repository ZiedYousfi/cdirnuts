#include "../include/dir_utils.h"

#include "../include/log.h"

char *constructPath(const char *dirName, const char *parentDir) {
  if (parentDir == NULL) {
    log_message(LOG_ERROR, "Parent directory cannot be NULL.");
    return NULL;
  }
  if (dirName == NULL) {
    log_message(LOG_ERROR, "Directory name cannot be NULL.");
    return NULL;
  }
  char fullPath[PATH_MAX];
  if (snprintf(fullPath, sizeof(fullPath), "%s/%s", parentDir, dirName) >=
      (int)sizeof(fullPath)) {
    log_message(LOG_ERROR, "Full path is too long.");
    return NULL;
  }
  return strdup(fullPath);
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

  char *fullPath = constructPath(dirName, parentDir);
  if (fullPath == NULL) {
    log_message(LOG_ERROR, "Failed to construct the full path.");
    return -1;
  }

  int statut = mkdir(fullPath, 0755);
  if (statut == 0) {
    log_message(LOG_INFO, "Directory %s was successfully created in %s!",
                dirName, parentDir);
  } else {
    log_message(LOG_ERROR, "Error creating directory");
    log_message(LOG_ERROR, "Error code: %d", statut);
    free(fullPath);
    return -1;
  }

  free(fullPath);
  return 0;
}

FILE *createFile(const char *fileName, const char *parentDir) {
  char *filePath = constructPath(fileName, parentDir);

  FILE *newFile = fopen(fileName, "w");
  if (newFile == NULL) {
    log_message(LOG_ERROR, "Failed to create file %s in %s", fileName,
                parentDir);
    free(filePath);
    return NULL;
  }

  free(filePath);
  return newFile;
}
