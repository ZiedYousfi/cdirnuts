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
    createDir(&dir->subDirs[i]);
  }

  for (size_t i = 0; i < dir->fileCount; i++) {
    createFile(&dir->files[i]);
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

int addDirToDir(cdirnutsDir *parent, cdirnutsDir *child) {
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
  parent->subDirs[parent->subDirCount] = *child;
  parent->subDirCount++;

  return 0;
}

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
  dir->files[dir->fileCount] = *file;
  dir->fileCount++;

  return 0;
}
