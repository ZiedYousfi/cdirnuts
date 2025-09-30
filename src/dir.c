#include "../include/dir.h"

int createDir(cdirnutsDir *dir) {
  if (!dir || !dir->path) {
    log_error("Invalid directory structure.");
    return -1;
  }

  int status = mkdir(dir->path, 0755);
  if (status == 0) {
    log_info("Directory %s was successfully created!", dir->path);
  } else {
    log_error("Error creating directory %s", dir->path);
    return -1;
  }

  for (size_t i = 0; i < dir->subDirCount; i++) {
    if (createDir(&dir->subDirs[i]) != 0) {
      log_error("Failed to create subdirectory");
      return -1;
    }
  }

  for (size_t i = 0; i < dir->fileCount; i++) {
    if (createFile(&dir->files[i]) != 0) {
      log_error("Failed to create file");
      return -1;
    }
  }

  return 0;
}

int createFile(cdirnutsFile *file) {
  if (!file || !file->path) {
    log_error("Invalid file structure.");
    return -1;
  }

  FILE *fp = fopen(file->path, "w");
  if (!fp) {
    log_error("Error creating file %s", file->path);
    return -1;
  }

  if (file->content) {
    size_t written =
        fwrite(file->content, sizeof(char), strlen(file->content), fp);
    if (written < strlen(file->content)) {
      log_error("Error writing to file %s", file->path);
      fclose(fp);
      return -1;
    }
  }

  fclose(fp);
  log_info("File %s was successfully created!", file->path);
  return 0;
}

int modifyFileContent(FILE *file, const char *content) {
  if (!file || !content) {
    log_error("Invalid file or content.");
    return -1;
  }

  rewind(file);

  size_t written = fwrite(content, sizeof(char), strlen(content), file);
  if (written < strlen(content)) {
    log_error("Error writing to file");
    return -1;
  }

  log_info("File content was successfully modified!");
  return 0;
}

/**
 * Adds a subdirectory to a parent directory with transfer-of-ownership
 * semantics.
 *
 * IMPORTANT: This function takes ownership of the child directory structure.
 * After this function returns successfully, the caller MUST NOT:
 * - Free the child structure or any of its contents
 * - Access or modify the child structure
 * - Pass the child pointer to any other function
 *
 * The child's internal pointers (path, subDirs, files) are transferred to the
 * parent's subDirs array, and the child structure is cleared to prevent
 * double-free.
 *
 * @param parent The parent directory (must not be NULL)
 * @param child The child directory to add (must not be NULL, ownership is
 * transferred)
 * @return 0 on success, -1 on failure
 */
int addSubDirToDir(cdirnutsDir *parent, cdirnutsDir *child) {
  if (!parent || !child) {
    log_error("Invalid directory structure.");
    return -1;
  }

  cdirnutsDir *newSubDirs =
      realloc(parent->subDirs, (parent->subDirCount + 1) * sizeof(cdirnutsDir));
  if (!newSubDirs) {
    log_error("Memory allocation failed.");
    return -1;
  }

  parent->subDirs = newSubDirs;

  // Transfer ownership: move child's contents into the new slot
  parent->subDirs[parent->subDirCount].path = child->path;
  parent->subDirs[parent->subDirCount].subDirs = child->subDirs;
  parent->subDirs[parent->subDirCount].subDirCount = child->subDirCount;
  parent->subDirs[parent->subDirCount].files = child->files;
  parent->subDirs[parent->subDirCount].fileCount = child->fileCount;

  parent->subDirCount++;

  // Clear child's pointers to prevent double-free
  // Caller must not use or free child after this point
  child->path = NULL;
  child->subDirs = NULL;
  child->subDirCount = 0;
  child->files = NULL;
  child->fileCount = 0;

  return 0;
}

/**
 * Adds a file to a directory with transfer-of-ownership semantics.
 *
 * IMPORTANT: This function takes ownership of the file structure.
 * After this function returns successfully, the caller MUST NOT:
 * - Free the file structure or any of its contents
 * - Access or modify the file structure
 * - Pass the file pointer to any other function
 *
 * The file's internal pointers (path, content) are transferred to the
 * directory's files array, and the file structure is cleared to prevent
 * double-free.
 *
 * @param dir The directory (must not be NULL)
 * @param file The file to add (must not be NULL, ownership is transferred)
 * @return 0 on success, -1 on failure
 */
int addFileToDir(cdirnutsDir *dir, cdirnutsFile *file) {
  if (!dir || !file) {
    log_error("Invalid directory or file structure.");
    return -1;
  }

  cdirnutsFile *newFiles =
      realloc(dir->files, (dir->fileCount + 1) * sizeof(cdirnutsFile));
  if (!newFiles) {
    log_error("Memory allocation failed.");
    return -1;
  }

  dir->files = newFiles;

  // Transfer ownership: move file's contents into the new slot
  dir->files[dir->fileCount].path = file->path;
  dir->files[dir->fileCount].content = file->content;

  dir->fileCount++;

  // Clear file's pointers to prevent double-free
  // Caller must not use or free file after this point
  file->path = NULL;
  file->content = NULL;

  return 0;
}

cdirnutsDir *allocDir(const char *path) {
  if (!path) {
    log_error("Invalid path.");
    return NULL;
  }

  cdirnutsDir *dir = malloc(sizeof(cdirnutsDir));
  if (!dir) {
    log_error("Memory allocation failed.");
    return NULL;
  }

  dir->path = strdup(path);
  if (!dir->path) {
    log_error("Memory allocation for path failed.");
    free(dir);
    return NULL;
  }

  dir->subDirCount = 0;
  dir->subDirs = NULL;
  dir->fileCount = 0;
  dir->files = NULL;

  return dir;
}

int freeDir(cdirnutsDir *dir) {
  if (!dir) {
    return -1;
  }

  for (size_t i = 0; i < dir->subDirCount; i++) {
    freeDir(&dir->subDirs[i]);
  }

  for (size_t i = 0; i < dir->fileCount; i++) {
    free(dir->files[i].path);
    free(dir->files[i].content);
  }

  free(dir->files);
  free(dir);
  dir = NULL;
  return 0;
}
