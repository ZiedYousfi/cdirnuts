#include "../include/dir_utils.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/log.h"

/**
 * @brief Constructs a full path by combining the parent directory and the
 * directory/file name.
 *
 * @param dirName The name of the directory/file.
 * @param parentDir The path of the parent directory.
 * @return A newly allocated string containing the full path, or NULL on
 * failure.
 */
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

/**
 * @brief Creates a directory with the specified name in the given parent
 * directory.
 *
 * @param dirName The name of the directory to create.
 * @param parentDir The path of the parent directory where the new directory
 * will be created.
 * @return 0 on success, -1 on failure.
 */
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
