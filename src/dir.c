#include "../include/dir.h"

int createDir(const char* dirName, const char* parentDir) {
  if (parentDir == NULL) {
    log_message(LOG_ERROR, "Parent directory cannot be NULL.");
    return -1;
  }
  if (dirName == NULL) {
    log_message(LOG_ERROR, "Directory name cannot be NULL.");
    return -1;
  }

  char* fullPath = constructPath(dirName, parentDir);
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

FILE* createFile(const char* fileName, const char* parentDir) {
  char* filePath = constructPath(fileName, parentDir);

  log_message(LOG_INFO, "Creating file %s in %s", fileName, parentDir);

  FILE* newFile = fopen(filePath, "w");
  if (newFile == NULL) {
    log_message(LOG_ERROR, "Failed to create file %s in %s", fileName,
                parentDir);
    free(filePath);
    return NULL;
  }

  log_message(LOG_INFO, "File %s was successfully created in %s!", fileName,
              parentDir);
  if (fputs("// This is a generated file by cdirnuts.\n", newFile) == EOF) {
    log_message(LOG_ERROR, "Failed to write to file %s", fileName);
    fclose(newFile);
    free(filePath);
    return NULL;
  }

  free(filePath);
  return newFile;
}

FILE* modifyFileContent(FILE* file, const char* content) {
  if (file == NULL || content == NULL) {
    log_message(LOG_ERROR, "File pointer or content is NULL.");
    return NULL;
  }
  rewind(file);

  if (fputs(content, file) == EOF) {
    log_message(LOG_ERROR, "Failed to write to file.");
    return NULL;
  }
  if (fflush(file) != 0 || ftruncate(fileno(file), ftell(file)) != 0) {
    log_message(LOG_ERROR, "Failed to flush or truncate the file.");
    return NULL;
  }

  return file;
}
