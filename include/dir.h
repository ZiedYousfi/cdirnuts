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

typedef struct {
  char *path;
  char *content;
} cdirnutsFile;

typedef struct {
  char *path;
  size_t subDirCount;
  cdirnutsDir *subDirs;
  cdirnutsFile *files;
  size_t fileCount;
} cdirnutsDir;

int createDir(cdirnutsDir *dir);

FILE *createFile(cdirnutsFile *file);

FILE *modifyFileContent(FILE *file, const char *content);
