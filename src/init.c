#include "../include/init.h"
#include "../include/path.h"

/**
 * Initialize a default C project layout under a given parent directory.
 *
 * Creates a project directory named by projectName containing src, include, and tests
 * subdirectories and populates the tree with a README.md, src/main.c, .gitignore,
 * and CMakeLists.txt (using projectName where appropriate).
 *
 * @param parentDir Path to the directory in which the project directory will be created.
 * @param projectName Name of the project; used as the project directory name and embedded
 *                    in generated files (README, main.c greeting, and CMakeLists).
 * @returns 0 on success, -1 on error (invalid inputs, allocation or path construction failures,
 *          or failure to add files/subdirectories). */
int init_default_setup(const char *parentDir, const char *projectName) {
  if (!parentDir || !projectName) {
    log_error("parentDir and projectName must be provided");
    return -1;
  }

  if (strlen(projectName) > 100) {
    log_error("projectName is too long (max 100 characters)");
    return -1;
  }

  int result = 0;
  cdirnutsDir *projectDir = NULL;
  cdirnutsDir *srcDir = NULL;
  cdirnutsDir *includeDir = NULL;
  cdirnutsDir *testsDir = NULL;
  char *projectPath = NULL;
  char *srcPath = NULL;
  char *includePath = NULL;
  char *testsPath = NULL;
  char *readmePath = NULL;
  char *readmeContent = NULL;
  char *mainPath = NULL;
  char *mainContent = NULL;
  char *gitignorePath = NULL;
  char *gitignoreContent = NULL;
  char *cmakePath = NULL;
  char *cmakeContent = NULL;

  projectPath = constructPath(projectName, parentDir);
  if (!projectPath) {
    log_error("Failed to construct project path");
    result = -1;
    goto cleanup;
  }

  projectDir = allocDir(projectPath);

  srcPath = constructPath("src", projectPath);
  includePath = constructPath("include", projectPath);
  testsPath = constructPath("tests", projectPath);

  if (!srcPath || !includePath || !testsPath) {
    log_error("Failed to construct subdirectory paths");
    result = -1;
    goto cleanup;
  }

  srcDir = allocDir(srcPath);
  includeDir = allocDir(includePath);
  testsDir = allocDir(testsPath);

  if (!srcDir || !includeDir || !testsDir) {
    log_error("Failed to allocate subdirectory structures");
    result = -1;
    goto cleanup;
  }

  if (addSubDirToDir(projectDir, srcDir) != 0 ||
      addSubDirToDir(projectDir, includeDir) != 0 ||
      addSubDirToDir(projectDir, testsDir) != 0) {
    log_error("Failed to add subdirectories to project");
    result = -1;
    goto cleanup;
  }

  readmePath = constructPath("README.md", projectPath);
  if (!readmePath) {
    log_error("Failed to construct README path");
    result = -1;
    goto cleanup;
  }

  readmeContent = malloc(512);
  if (!readmeContent) {
    log_error("Failed to allocate memory for README content");
    result = -1;
    goto cleanup;
  }
  snprintf(readmeContent, 512, "# %s\n\nProject initialized by cdirnuts.\n",
           projectName);

  cdirnutsFile readmeFile = {.path = readmePath, .content = readmeContent};

  if (addFileToDir(projectDir, &readmeFile) != 0) {
    log_error("Failed to add README to project");
    result = -1;
    goto cleanup;
  }

  // Create main.c in src directory
  mainPath = constructPath("main.c", srcPath);
  if (!mainPath) {
    log_error("Failed to construct main.c path");
    result = -1;
    goto cleanup;
  }

  mainContent = malloc(512);
  if (!mainContent) {
    log_error("Failed to allocate memory for main.c content");
    result = -1;
    goto cleanup;
  }
  snprintf(mainContent, 512,
           "#include <stdio.h>\n\n"
           "int main(int argc, char *argv[]) {\n"
           "    printf(\"Hello from %s!\\n\");\n"
           "    return 0;\n"
           "}\n",
           projectName);

  cdirnutsFile mainFile = {.path = mainPath, .content = mainContent};

  if (addFileToDir(srcDir, &mainFile) != 0) {
    log_error("Failed to add main.c to src directory");
    result = -1;
    goto cleanup;
  }

  // Create .gitignore in project root
  gitignorePath = constructPath(".gitignore", projectPath);
  if (!gitignorePath) {
    log_error("Failed to construct .gitignore path");
    result = -1;
    goto cleanup;
  }

  gitignoreContent = malloc(512);
  if (!gitignoreContent) {
    log_error("Failed to allocate memory for .gitignore content");
    result = -1;
    goto cleanup;
  }
  snprintf(gitignoreContent, 512,
           "# Build directories\n"
           "build/\n"
           "*.o\n"
           "*.a\n"
           "*.so\n"
           "*.dylib\n\n"
           "# IDE files\n"
           ".vscode/\n"
           ".idea/\n\n"
           "# OS files\n"
           ".DS_Store\n"
           "Thumbs.db\n");

  cdirnutsFile gitignoreFile = {.path = gitignorePath,
                                .content = gitignoreContent};

  if (addFileToDir(projectDir, &gitignoreFile) != 0) {
    log_error("Failed to add .gitignore to project");
    result = -1;
    goto cleanup;
  }

  // Create CMakeLists.txt in project root
  cmakePath = constructPath("CMakeLists.txt", projectPath);
  if (!cmakePath) {
    log_error("Failed to construct CMakeLists.txt path");
    result = -1;
    goto cleanup;
  }

  cmakeContent = malloc(512);
  if (!cmakeContent) {
    log_error("Failed to allocate memory for CMakeLists.txt content");
    result = -1;
    goto cleanup;
  }
  snprintf(cmakeContent, 512,
           "cmake_minimum_required(VERSION 3.10)\n"
           "project(%s C)\n\n"
           "set(CMAKE_C_STANDARD 23)\n\n"
           "include_directories(include)\n\n"
           "add_executable(%s src/main.c)\n",
           projectName, projectName);

  cdirnutsFile cmakeFile = {.path = cmakePath, .content = cmakeContent};

  if (addFileToDir(projectDir, &cmakeFile) != 0) {
    log_error("Failed to add CMakeLists.txt to project");
    result = -1;
    goto cleanup;
  }
  result = createDir(projectDir);

cleanup:
  if (projectDir)
    freeDir(projectDir);
  if (readmeContent)
    free(readmeContent);
  if (mainContent)
    free(mainContent);
  if (gitignoreContent)
    free(gitignoreContent);
  if (cmakeContent)
    free(cmakeContent);
  if (projectPath)
    free(projectPath);
  if (srcPath)
    free(srcPath);
  if (includePath)
    free(includePath);
  if (testsPath)
    free(testsPath);
  if (readmePath)
    free(readmePath);
  if (mainPath)
    free(mainPath);
  if (gitignorePath)
    free(gitignorePath);
  if (cmakePath)
    free(cmakePath);

  return result;
}
