#include "../include/dir.h"

int createDir(const char *dirName, const char *parentDir) {
  if (parentDir == NULL) {
    log_error("Parent directory cannot be NULL.");
    return -1;
  }
  if (dirName == NULL) {
    log_error("Directory name cannot be NULL.");
    return -1;
  }

  char *fullPath = constructPath(dirName, parentDir);
  if (fullPath == NULL) {
    log_error("Failed to construct the full path.");
    return -1;
  }

  int statut = mkdir(fullPath, 0755);
  if (statut == 0) {
    log_info("Directory %s was successfully created in %s!", dirName,
             parentDir);
  } else {
    log_error("Error creating directory");
    log_error("Error code: %d", statut);
    free(fullPath);
    return -1;
  }

  free(fullPath);
  return 0;
}

FILE *createFile(const char *fileName, const char *parentDir) {
  if (fileName == NULL || parentDir == NULL) {
    log_error("File name or parent directory is NULL.");
    return NULL;
  }

  FILE *newFile = NULL;
  char *filePath = constructPath(fileName, parentDir);

  log_info("Creating file %s in %s", fileName, parentDir);

  newFile = fopen(filePath, "w");
  if (newFile == NULL) {
    log_error("Failed to create file %s in %s", fileName, parentDir);
    goto cleanup;
  }

  log_info("File %s was successfully created in %s!", fileName, parentDir);
  if (fputs("// This is a generated file by cdirnuts.\n", newFile) == EOF) {
    log_error("Failed to write to file %s", fileName);
    goto cleanup;
  }

cleanup:
  free(filePath);
  if (newFile) {
    fclose(newFile);
    return NULL;
  }

  return newFile;
}

FILE *modifyFileContent(FILE *file, const char *content) {
  if (file == NULL || content == NULL) {
    log_error("File pointer or content is NULL.");
    return NULL;
  }
  rewind(file);

  if (fputs(content, file) == EOF) {
    log_error("Failed to write to file.");
    return NULL;
  }
  if (fflush(file) != 0 || ftruncate(fileno(file), ftell(file)) != 0) {
    log_error("Failed to flush or truncate the file.");
    return NULL;
  }

  return file;
}
