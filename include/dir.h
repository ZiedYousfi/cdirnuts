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

int addSubDirToDir(cdirnutsDir *parentDir, cdirnutsDir *subDir);

int addFileToDir(cdirnutsDir *dir, cdirnutsFile *file);

cdirnutsDir *allocDir(const char *path);

int freeDir(cdirnutsDir *dir);
