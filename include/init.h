#pragma once

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "./dir.h"
#include "./log.h"

/**
 * @brief Initializes the application by setting up the default
 *
 * @return 0 on success, -1 on failure.
 */

int init_default_setup(const char *parentDir, const char *projectName);
