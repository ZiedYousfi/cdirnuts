#ifndef DIR_UTILS_H
#define DIR_UTILS_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/path_utils.h"
#include "../include/log.h"

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

/**
 * @brief Creates a file with the specified name in the given parent directory.
 *
 * @param fileName The name of the file to create.
 * @param parentDir The path of the parent directory where the new file will be
 * created.
 * @return A pointer to the created file, or NULL on failure.
 */
FILE *createFile(const char *fileName, const char *parentDir);

/**
 * @brief Modifies the content of a file by writing the specified content to it.
 *
 * @param filePath The path of the file to modify.
 * @param content The content to write to the file.
 * @return A pointer to the modified file, or NULL on failure.
 */
FILE *modifyFileContent(FILE *file, const char *content);

#endif /* DIR_UTILS_H */
