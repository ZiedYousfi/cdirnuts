#include <limits.h>
#include <unistd.h>

#include "../include/dir_utils.h"
#include "../include/init.h"
#include "../include/log.h"
#include "../include/path_utils.h"

#define PROJECT_NAME "my_project"

int main(int argc, char *argv[]) {
  int result = 0;
  bool shouldFreeProjectName = false;
  char *projectName = PROJECT_NAME;

  if (argc > 1) {
    if (argv[1][0] == '-') {
      if (strcmp(argv[1], "--help") == 0) {
        /* code for help */
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
      projectName = argv[1];
      char cwd[PATH_MAX];
      if (strchr(projectName, '/')) {
        // TODO: Handle case where projectName contains a path
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
  if (shouldFreeProjectName) free(projectName);
  return result;

  // char pathTest[] = "/home/user/projects/my_project/src/main/";
  // PathInfo *pathInfo = parsePath(pathTest);
  // if (pathInfo) {
  //   printf("Parent Path: %s\n", pathInfo->parentPath);
  //   printf("Name: %s\n", pathInfo->name);
  //   printf("Is Directory: %s\n", pathInfo->isDirectory ? "true" : "false");
  //   freePathInfo(pathInfo);
  // } else {
  //   printf("Failed to parse path.\n");
  // }
  // return 0;
  // Uncomment the following lines to test the directory and file creation
  // if (createDir("test_dir", ".")) {
  //   printf("Directory created successfully.\n");
  // } else {
  //   printf("Failed to create directory.\n");
  // }
  // FILE *file = createFile("test_file.txt", ".");
  // if (file) {
  //   printf("File created successfully.\n");
  //   fclose(file);
  // } else {
  //   printf("Failed to create file.\n");
  // }
  // return 0;
  // Uncomment the following lines to test the default setup initialization
  // if (init_default_setup(".", "my_project") != 0) {
  //   printf("Failed to initialize default setup.\n");
  //   return 1;
  // }
  // printf("Default setup initialized successfully.\n");
  // return 0;
}
