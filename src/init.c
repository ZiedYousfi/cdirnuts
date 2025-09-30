#include "../include/init.h"
#include "../include/path.h"

int init_default_setup(const char *parentDir, const char *projectName) {
  if (!parentDir || !projectName) {
    log_error("parentDir and projectName must be provided");
    return -1;
  }

  int result = 0;
  char *projectPath = NULL;
  char *srcPath = NULL;
  char *includePath = NULL;
  char *testsPath = NULL;
  char *readmePath = NULL;
  char *readmeContent = NULL;

  projectPath = constructPath(projectName, parentDir);
  if (!projectPath) {
    log_error("Failed to construct project path");
    return -1;
  }

  cdirnutsDir projectDir = {.path = projectPath,
                            .subDirCount = 0,
                            .subDirs = NULL,
                            .fileCount = 0,
                            .files = NULL};

  srcPath = constructPath("src", projectPath);
  includePath = constructPath("include", projectPath);
  testsPath = constructPath("tests", projectPath);

  if (!srcPath || !includePath || !testsPath) {
    log_error("Failed to construct subdirectory paths");
    free(projectPath);
    free(srcPath);
    free(includePath);
    free(testsPath);
    return -1;
  }

  cdirnutsDir srcDir = {.path = srcPath,
                        .subDirCount = 0,
                        .subDirs = NULL,
                        .fileCount = 0,
                        .files = NULL};

  cdirnutsDir includeDir = {.path = includePath,
                            .subDirCount = 0,
                            .subDirs = NULL,
                            .fileCount = 0,
                            .files = NULL};

  cdirnutsDir testsDir = {.path = testsPath,
                          .subDirCount = 0,
                          .subDirs = NULL,
                          .fileCount = 0,
                          .files = NULL};

  if (addSubDirToDir(&projectDir, &srcDir) != 0 ||
      addSubDirToDir(&projectDir, &includeDir) != 0 ||
      addSubDirToDir(&projectDir, &testsDir) != 0) {
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

  if (addFileToDir(&projectDir, &readmeFile) != 0) {
    log_error("Failed to add README to project");
    result = -1;
    goto cleanup;
  }

  result = createDir(&projectDir);

cleanup:
  if (readmeContent)
    free(readmeContent);
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

  return result;
}
