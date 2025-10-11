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
  char *value = malloc(valueLen + 1);
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
  /*
   * Need space for: opening quote, name, closing quote, comma, opening quote,
   * path, closing quote and terminating NUL.
   */
  size_t name_len = preset.name ? strlen(preset.name) : 0;
  size_t path_len = preset.path ? strlen(preset.path) : 0;
  char *result = malloc(name_len + path_len + 6); // +6 for quotes, comma, NUL
  if (!result) {
    return NULL;
  }
  sprintf(result, "\"%s\",\"%s\"", preset.name, preset.path);
  return result;
}

int addPreset(const char *name, const char *path) {
  if (!name || !path) {
    return -1;
  }

  // Load existing presets
  Preset *presets = getPresets();
  int count = 0;

  if (presets) {
    // Count existing presets
    while (presets[count].name != NULL) {
      // Check if preset already exists
      if (strcmp(presets[count].name, name) == 0) {
        // Free the existing presets
        for (int i = 0; presets[i].name != NULL; i++) {
          free(presets[i].name);
          free(presets[i].path);
        }
        free(presets);
        return -2; // Preset already exists
      }
      count++;
    }
  }

  // Allocate space for new preset array
  Preset *newPresets = (Preset *)realloc(presets, (count + 2) * sizeof(Preset));
  if (!newPresets) {
    if (presets) {
      for (int i = 0; i < count; i++) {
        free(presets[i].name);
        free(presets[i].path);
      }
      free(presets);
    }
    return -1;
  }

  // Add new preset
  newPresets[count].name = strdup(name);
  newPresets[count].path = strdup(path);
  newPresets[count + 1].name = NULL;
  newPresets[count + 1].path = NULL;

  if (!newPresets[count].name || !newPresets[count].path) {
    free(newPresets[count].name);
    free(newPresets[count].path);
    for (int i = 0; i < count; i++) {
      free(newPresets[i].name);
      free(newPresets[i].path);
    }
    free(newPresets);
    return -1;
  }

  // Save presets
  int result = savePresets(newPresets, count + 1);

  // Free memory
  for (int i = 0; i <= count; i++) {
    free(newPresets[i].name);
    free(newPresets[i].path);
  }
  free(newPresets);

  return result;
}

int removePreset(const char *name) {
  if (!name) {
    return -1;
  }

  // Load existing presets
  Preset *presets = getPresets();
  if (!presets) {
    return -1; // No presets found
  }

  int count = 0;
  int foundIndex = -1;

  // Count presets and find the one to remove
  while (presets[count].name != NULL) {
    if (strcmp(presets[count].name, name) == 0) {
      foundIndex = count;
    }
    count++;
  }

  if (foundIndex == -1) {
    // Preset not found, free memory and return
    for (int i = 0; i < count; i++) {
      free(presets[i].name);
      free(presets[i].path);
    }
    free(presets);
    return -2; // Preset not found
  }

  // Create new array without the removed preset
  Preset *newPresets = NULL;
  if (count > 1) {
    newPresets = (Preset *)malloc(count * sizeof(Preset));
    if (!newPresets) {
      for (int i = 0; i < count; i++) {
        free(presets[i].name);
        free(presets[i].path);
      }
      free(presets);
      return -1;
    }

    int newIndex = 0;
    for (int i = 0; i < count; i++) {
      if (i != foundIndex) {
        newPresets[newIndex].name = presets[i].name;
        newPresets[newIndex].path = presets[i].path;
        newIndex++;
      } else {
        free(presets[i].name);
        free(presets[i].path);
      }
    }
    newPresets[newIndex].name = NULL;
    newPresets[newIndex].path = NULL;
  }

  // Save updated presets
  int result = savePresets(newPresets, count - 1);

  // Free memory
  if (newPresets) {
    for (int i = 0; i < count - 1; i++) {
      free(newPresets[i].name);
      free(newPresets[i].path);
    }
    free(newPresets);
  }
  free(presets);

  return result;
}

void listPresets() {
  Preset *presets = getPresets();

  if (!presets) {
    printf("No presets found.\n");
    return;
  }

  int count = 0;
  while (presets[count].name != NULL) {
    count++;
  }

  if (count == 0) {
    printf("No presets found.\n");
  } else {
    printf("Available presets:\n");
    for (int i = 0; i < count; i++) {
      printf("  %s -> %s\n", presets[i].name, presets[i].path);
    }
  }

  // Free memory
  for (int i = 0; i < count; i++) {
    free(presets[i].name);
    free(presets[i].path);
  }
  free(presets);
}
