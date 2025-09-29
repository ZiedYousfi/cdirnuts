#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char *parentPath;
  char *name;
  bool isDirectory;
} PathInfo;

/**
 * @brief Copies a substring from the source string.
 *
 * Assumes callers call free() on the returned string when done.
 *
 * @param source The source string from which to copy the substring.
 * @param start The starting index of the substring (inclusive).
 * @param end The ending index of the substring (inclusive).
 * @return A newly allocated string containing the substring, or NULL on
 * failure.
 */
char *copySubstring(const char *source, int start, int end);

/**
 * @brief Constructs a full path by combining the parent directory and the
 * directory/file name.
 *
 * Assumes callers call free() on the returned string when done.
 *
 * @param dirName The name of the directory/file.
 * @param parentDir The path of the parent directory.
 * @return A newly allocated string containing the full path, or NULL on
 * failure.
 */
char *constructPath(const char *dirName, const char *parentDir);

/**
 * @brief Parses a path string into its components.
 *
 * Assumes callers call free() on the returned PathInfo structure when done.
 *
 * @param path The path string to parse.
 * @return A pointer to a newly allocated PathInfo structure containing the
 * parsed components, or NULL on failure.
 */
PathInfo *parsePath(const char *path);

/**
 * @brief Creates a new PathInfo structure.
 *
 * Assumes callers call free() on the returned PathInfo structure when done.
 *
 * @return A pointer to a newly allocated PathInfo structure, or NULL on
 * failure.
 */
PathInfo *createPathInfo();

/**
 * @brief Frees the memory allocated for a PathInfo structure.
 *
 * @param toFree The PathInfo structure to free.
 */
void freePathInfo(PathInfo *toFree);
