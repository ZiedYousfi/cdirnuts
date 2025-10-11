#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Structure representing a preset configuration with a name and associated
 * path. Will be formated as "name","path" and separated by a new line in .cdndb
 * file.
 */
typedef struct Preset {
  char *name;
  char *path;
} Preset;

Preset *getPresets();
int savePresets(Preset *presets, int count);
Preset fromString(const char *str);
char *toString(Preset preset);
int addPreset(const char *name, const char *path);
int removePreset(const char *name);
void listPresets();
