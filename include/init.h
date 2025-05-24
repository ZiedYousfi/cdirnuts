#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dir_utils.h"
#include "./log.h"

/**
 * @brief Initializes the application by setting up the default
 *
 * @return 0 on success, -1 on failure.
 */

int init_default_setup(const char *parentDir, const char *projectName);

#endif // INIT_H
