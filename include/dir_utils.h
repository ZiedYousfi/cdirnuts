#ifndef DIR_UTILS_H
#define DIR_UTILS_H

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
 * @brief Creates a directory with the specified name in the given parent
 * directory.
 *
 * @param dirName The name of the directory to create.
 * @param parentDir The path of the parent directory where the new directory
 * will be created.
 * @return 0 on success, -1 on failure.
 */
int createDir(const char *dirName, const char *parentDir);

#endif /* DIR_UTILS_H */
