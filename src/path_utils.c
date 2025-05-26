#include "../include/path_utils.h"

#include "../include/log.h"

char *copySubstring(const char *source, int start, int end) {
  if (source == NULL || start < 0 || end < start ||
      end >= (int)strlen(source)) {
    log_message(LOG_ERROR, "Invalid parameters for substring copy.");
    return NULL;
  }
  int length = end - start + 1;
  char *substring = malloc(length + 1);
  if (substring == NULL) {
    log_message(LOG_ERROR, "Memory allocation failed for substring.");
    return NULL;
  }
  strncpy(substring, source + start, length);
  substring[length] = '\0';
  return substring;
}

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

PathInfo *parsePath(const char *path) {
  bool shouldFreeResult = false;
  PathInfo *result = createPathInfo();
  char *name = NULL;

  if (!path || path[0] == '\0') {
    log_message(LOG_ERROR, "Path cannot be NULL or empty.");
    shouldFreeResult = true;
    goto cleanup_parsePath;
  }

  int pathLen = strlen(path);

  if (pathLen > 0 && path[pathLen - 1] == '/') {
    result->isDirectory = true;
  }

  int pathLastSlashIndex = -1;
  for (int i = pathLen - 1; i >= 0; i--) {
    if (path[i] == '/') {
      pathLastSlashIndex = i;
      break;
    }
  }

  int nameSize = (pathLastSlashIndex == -1) ? pathLen
                                            : pathLen - pathLastSlashIndex -
                                                  (result->isDirectory ? 1 : 0);
  if (result->isDirectory && pathLastSlashIndex == pathLen - 1) {
    for (int i = pathLen - 2; i >= 0; i--) {
      if (path[i] == '/') {
        pathLastSlashIndex = i;
        break;
      }
    }
    nameSize = pathLen - pathLastSlashIndex - 1;
  }
  if (nameSize > 0) {
    name = calloc(nameSize + 1, sizeof(char));
    if (!name) {
      log_message(LOG_ERROR, "Memory allocation failed for name.");
      shouldFreeResult = true;
      goto cleanup_parsePath;
    }
    strncpy(name, path + pathLastSlashIndex + 1, nameSize);
    name[nameSize] = '\0';
  }

  result->name = name;
  name = NULL;

cleanup_parsePath:

  if (name) free(name);

  if (shouldFreeResult) {
    freePathInfo(result);
    result = NULL;
  }

  return result;
}

PathInfo *createPathInfo() {
  PathInfo *result = malloc(sizeof(PathInfo));
  if (result == NULL) {
    log_message(LOG_ERROR, "Memory allocation failed for PathInfo.");
    return NULL;
  }
  result->parentPath = NULL;
  result->name = NULL;
  result->isDirectory = false;

  return result;
}

void freePathInfo(PathInfo *toFree) {
  if (toFree->name) free(toFree->name);
  if (toFree->parentPath) free(toFree->parentPath);
  free(toFree);
  toFree = NULL;
  log_message(LOG_INFO, "PathInfo structure freed successfully.");
}
