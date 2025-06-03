#include "../include/dir_utils.h"
#include "../include/init.h"
#include "../include/log.h"
#include "../include/path_utils.h"

int tests(){

char pathTest[] = "/home/user/projects/my_project/src/main/";
  PathInfo *pathInfo = parsePath(pathTest);
  if (pathInfo) {
    printf("Parent Path: %s\n", pathInfo->parentPath);
    printf("Name: %s\n", pathInfo->name);
    printf("Is Directory: %s\n", pathInfo->isDirectory ? "true" : "false");
    freePathInfo(pathInfo);
  } else {
    printf("Failed to parse path.\n");
  }
  return 0;

  if (createDir("test_dir", ".")) {
    printf("Directory created successfully.\n");
  } else {
    printf("Failed to create directory.\n");
  }
  FILE *file = createFile("test_file.txt", ".");
  if (file) {
    printf("File created successfully.\n");
    fclose(file);
  } else {
    printf("Failed to create file.\n");
  }
  return 0;

  if (init_default_setup(".", "my_project") != 0) {
    printf("Failed to initialize default setup.\n");
    return 1;
  }
  printf("Default setup initialized successfully.\n");
  return 0;
}
