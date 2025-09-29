#include "../include/init.h"

int init_default_setup(const char* parentDir, const char* projectName) {
  int result = 0;
  FILE* mainFile = NULL;
  char* projectDir = NULL;
  char* srcPath = NULL;
  FILE* gitignoreFile = NULL;
  FILE* readmeFile = NULL;
  FILE* makefileFile = NULL;
  FILE* licenseFile = NULL;
  // FILE *authorsFile = NULL;

  if (parentDir == NULL || projectName == NULL) {
    log_message(LOG_ERROR, "Parent directory or project name cannot be NULL.");
    return -1;
  }
  if (strlen(projectName) == 0) {
    log_message(LOG_ERROR, "Project name cannot be an empty string.");
    return -1;
  }
  if (strlen(parentDir) == 0) {
    log_message(LOG_ERROR, "Parent directory cannot be an empty string.");
    return -1;
  }

  if (createDir(projectName, parentDir)) {
    log_message(LOG_ERROR, "Failed to create project directory.");
    return -1;
  }

  projectDir = constructPath(projectName, parentDir);
  if (projectDir == NULL) {
    log_message(LOG_ERROR, "Failed to construct the project directory path.");
    result = -1;
    goto cleanup_init_default_setup;
  }

  log_message(LOG_INFO, "Project directory '%s' created successfully in '%s'.",
              projectName, parentDir);
  log_message(LOG_INFO, "Project directory path: %s", projectDir);

  if (createDir("include", projectDir)) {
    log_message(LOG_ERROR, "Failed to create include directory.");
    result = -1;
    goto cleanup_init_default_setup;
  }

  if (createDir("src", projectDir)) {
    log_message(LOG_ERROR, "Failed to create src directory.");
    result = -1;
    goto cleanup_init_default_setup;
  }

  srcPath = constructPath("src", projectDir);
  if (srcPath == NULL) {
    log_message(LOG_ERROR, "Failed to construct src directory path.");
    result = -1;
    goto cleanup_init_default_setup;
  }

  mainFile = createFile("main.c", srcPath);
  if (mainFile == NULL) {
    log_message(LOG_ERROR, "Failed to create main.c file.");
    result = -1;
    goto cleanup_init_default_setup;
  }

  mainFile =
      modifyFileContent(mainFile,
                        "#include <stdio.h>\n\nint main() {\n    "
                        "printf(\"Hello, World!\\n\");\n    return 0;\n}\n");
  if (mainFile == NULL) {
    log_message(LOG_ERROR, "Failed to modify main.c file content.");
    result = -1;
    goto cleanup_init_default_setup;
  }
  log_message(LOG_INFO, "main.c file created and modified successfully.");

  gitignoreFile = createFile(".gitignore", projectDir);
  if (gitignoreFile == NULL) {
    log_message(LOG_ERROR, "Failed to create .gitignore file.");
    result = -1;
    goto cleanup_init_default_setup;
  }
  gitignoreFile =
      modifyFileContent(gitignoreFile, "build/\n*.o\n*.exe\n*.out\n*.log\n");
  if (gitignoreFile == NULL) {
    log_message(LOG_ERROR, "Failed to modify .gitignore file content.");
    result = -1;
    goto cleanup_init_default_setup;
  }

  log_message(LOG_INFO, ".gitignore file created and modified successfully.");
  readmeFile = createFile("README.md", projectDir);
  if (readmeFile == NULL) {
    log_message(LOG_ERROR, "Failed to create README.md file.");
    result = -1;
    goto cleanup_init_default_setup;
  }
  char readmeBuffer[256];
  snprintf(readmeBuffer, sizeof(readmeBuffer),
           "# %s\n\nThis is a sample project.\n", projectName);
  readmeFile = modifyFileContent(readmeFile, readmeBuffer);
  if (readmeFile == NULL) {
    log_message(LOG_ERROR, "Failed to modify README.md file content.");
    result = -1;
    goto cleanup_init_default_setup;
  }
  log_message(LOG_INFO, "README.md file created and modified successfully.");
  makefileFile = createFile("Makefile", projectDir);
  if (makefileFile == NULL) {
    log_message(LOG_ERROR, "Failed to create Makefile file.");
    result = -1;
    goto cleanup_init_default_setup;
  }
  makefileFile = modifyFileContent(
      makefileFile,
      "CC = gcc\nCFLAGS = -Wall -Wextra -std=c17\n\nall: main\n\nmain: "
      "main.o\n\t$(CC) $(CFLAGS) -o main main.o\n\nmain.o: src/main.c\n\t$(CC) "
      "$(CFLAGS) -c src/main.c\n\nclean:\n\trm -f *.o main\n");
  if (makefileFile == NULL) {
    log_message(LOG_ERROR, "Failed to modify Makefile file content.");
    result = -1;
    goto cleanup_init_default_setup;
  }
  log_message(LOG_INFO, "Makefile file created and modified successfully.");
  licenseFile = createFile("LICENSE", projectDir);
  if (licenseFile == NULL) {
    log_message(LOG_ERROR, "Failed to create LICENSE file.");
    result = -1;
    goto cleanup_init_default_setup;
  }

cleanup_init_default_setup:
  if (licenseFile) fclose(licenseFile);
  if (makefileFile) fclose(makefileFile);
  if (readmeFile) fclose(readmeFile);
  if (gitignoreFile) fclose(gitignoreFile);
  if (mainFile) fclose(mainFile);
  if (srcPath) free(srcPath);
  if (projectDir) free(projectDir);
  return result;
}
