#include <limits.h>
#include <unistd.h>

#include "../include/dir_utils.h"
#include "../include/init.h"
#include "../include/log.h"
#include "../include/path_utils.h"

#define PROJECT_NAME "my_project"

void print_help() {
  printf("Usage: cdirnuts [options] [project_name]\n");
  printf("Options:\n");
  printf("  --help       Show this help message\n");
  printf("  --config <file>  Specify a configuration file\n");
  printf("If no project name is provided, 'my_project' will be used.\n");
}

int main(int argc, char *argv[]) {
  int result = 0;
  bool shouldFreeProjectName = false;
  char *projectName = PROJECT_NAME;
  PathInfo *pathInfo = NULL;

  if (argc > 1) {
    if (argv[1][0] == '-') {
      if (strcmp(argv[1], "--help") == 0) {
        print_help();
        goto cleanup_main;
      } else if (strcmp(argv[1], "--config") == 0) {
        if (argc <= 2) {
          log_message(
              LOG_ERROR,
              "--config as first argument but no config file path provided");
          result = 1;
          goto cleanup_main;
        }
        log_message(LOG_INFO, "Selected config : %s", argv[2]);
      }
    } else {
      char cwd[PATH_MAX];
      if (strchr(argv[1], '/')) {
        pathInfo = parsePath(argv[1]);
        if (pathInfo) {
          if (pathInfo->isDirectory) {
            log_message(LOG_INFO, "Project name is a directory: %s",
                        pathInfo->name);
          } else {
            projectName = pathInfo->name;
          }
          if (pathInfo->parentPath) {
            log_message(LOG_INFO, "Parent path: %s", pathInfo->parentPath);
            strncpy(cwd, pathInfo->parentPath, sizeof(cwd) - 1);
            cwd[sizeof(cwd) - 1] = '\0';
          } else {
            log_message(LOG_INFO, "No parent path found.");
          }
        } else {
          log_message(LOG_ERROR, "Failed to parse project name path.");
          result = 1;
          goto cleanup_main;
        }
        shouldFreeProjectName = true;
      } else {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          log_message(LOG_INFO, "Current directory at startup: %s", cwd);
        } else {
          log_message(LOG_ERROR, "getcwd() error");
          result = 1;
          goto cleanup_main;
        }
      }
      if (init_default_setup(cwd, projectName) != 0) {
        log_message(LOG_ERROR, "Failed to initialize default setup.");
        result = 1;
        goto cleanup_main;
      }
    }
  } else {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      log_message(LOG_INFO, "Current directory at startup: %s", cwd);
    } else {
      log_message(LOG_ERROR, "getcwd() error");
      result = 1;
      goto cleanup_main;
    }

    if (init_default_setup(cwd, "my_project") != 0) {
      log_message(LOG_ERROR, "Failed to initialize default setup.");
      result = 1;
      goto cleanup_main;
    }
  }

  log_message(LOG_INFO, "Hello, World!");

cleanup_main:
  if (pathInfo) free(pathInfo);
  if (shouldFreeProjectName) free(projectName);
  return result;

}
