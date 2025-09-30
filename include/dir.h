#pragma once

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/log.h"
#include "../include/path.h"

// Forward declaration
typedef struct cdirnutsDir cdirnutsDir;

typedef struct {
  char *path;
  char *content;
} cdirnutsFile;

struct cdirnutsDir {
  char *path;
  size_t subDirCount;
  cdirnutsDir *subDirs;
  cdirnutsFile *files;
  size_t fileCount;
};

int createDir(cdirnutsDir *dir);

int createFile(cdirnutsFile *file);

int modifyFileContent(FILE *file, const char *content);

/**
 * Adds a subdirectory to a parent directory.
 * IMPORTANT: This function takes ownership of the subDir parameter.
 * After calling, the caller must NOT free or use the subDir pointer.
 */
int addSubDirToDir(cdirnutsDir *parentDir, cdirnutsDir *subDir);

/**
 * Adds a file to a directory with deep-copy semantics.
 * The file's path and content are duplicated, and the caller retains
 * ownership of the original file structure and must free it if needed.
 */
int addFileToDir(cdirnutsDir *dir, cdirnutsFile *file);

cdirnutsDir *allocDir(const char *path);

int freeDir(cdirnutsDir *dir);
