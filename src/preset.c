#include "../include/preset.h"

static char *valueBetweenQuotes(const char *str, size_t start) {
  size_t len = strlen(str);
  if (start >= len || str[start] != '\"') {
    return NULL;
  }

  size_t end = start + 1;
  while (end < len && str[end] != '\"') {
    end++;
  }

  if (end >= len) {
    return NULL;
  }

  size_t valueLen = end - start - 1;
  char *value = (char *)malloc(valueLen + 1);
  if (!value) {
    return NULL;
  }

  strncpy(value, str + start + 1, valueLen);
  value[valueLen] = '\0';
  return value;
}

Preset *getPresets() {
  FILE *file = fopen("presets.cdndb", "r");
  if (!file) {
    return NULL;
  }
  Preset *presets = NULL;
  size_t count = 0;
  char line[512];

  while (fgets(line, sizeof(line), file)) {
    // Remove newline character
    line[strcspn(line, "\n")] = 0;

    Preset preset = fromString(line);
    if (preset.name && preset.path) {
      Preset *newPresets =
          (Preset *)realloc(presets, (count + 2) * sizeof(Preset));
      if (!newPresets) {
        // Free previously allocated presets
        for (size_t i = 0; i < count; i++) {
          free(presets[i].name);
          free(presets[i].path);
        }
        free(presets);
        fclose(file);
        return NULL;
      }
      presets = newPresets;
      presets[count] = preset;
      // maintain a NULL sentinel for easier counting by callers
      presets[count + 1].name = NULL;
      presets[count + 1].path = NULL;
      count++;
    }
  }

  fclose(file);
  return presets;
}

int savePresets(Preset *presets, int count) {
  FILE *file = fopen("presets.cdndb", "w");

  if (!file) {
    return -1;
  }

  for (int i = 0; i < count; i++) {
    char *presetStr = toString(presets[i]);
    if (!presetStr) {
      fclose(file);
      return -1;
    }
    fprintf(file, "%s\n", presetStr);
    free(presetStr);
  }
  fclose(file);
  return 0;
}

Preset fromString(const char *str) {
  Preset preset = {NULL, NULL};
  size_t len = strlen(str);
  if (len == 0) {
    return preset;
  }

  // Count quotes quickly to do a cheap sanity check
  int numberOfQuotes = 0;
  for (size_t i = 0; i < len; i++) {
    if (str[i] == '"') {
      numberOfQuotes++;
    }
  }

  if (numberOfQuotes < 2) {
    return preset;
  }

  // find first quoted value (name)
  size_t i = 0;
  while (i < len && str[i] != '"')
    i++;
  if (i >= len)
    return preset;
  char *name = valueBetweenQuotes(str, i);
  if (!name)
    return preset;
  preset.name = name;

  // find next quote after the end of the first quoted value
  size_t j = i + 1;
  while (j < len && str[j] != '"')
    j++; // j is at closing quote
  if (j >= len) {
    free(preset.name);
    preset.name = NULL;
    return preset;
  }
  // search for opening quote of path
  size_t k = j + 1;
  while (k < len && str[k] != '"')
    k++;
  if (k >= len) {
    free(preset.name);
    preset.name = NULL;
    return preset;
  }
  char *path = valueBetweenQuotes(str, k);
  if (!path) {
    free(preset.name);
    preset.name = NULL;
    return preset;
  }
  preset.path = path;
  return preset;
}

char *toString(Preset preset) {
  char *result = (char *)malloc(strlen(preset.name) + strlen(preset.path) + 5);
  if (!result) {
    return NULL;
  }
  sprintf(result, "\"%s\",\"%s\"", preset.name, preset.path);
  return result;
}
