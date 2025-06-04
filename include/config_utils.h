#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include "./log.h"
#include "./dir_utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int executeCommand(const char *command);


#endif // CONFIG_UTILS_H
