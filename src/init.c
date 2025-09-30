#include "../include/init.h"
#include "../include/path.h"

int init_default_setup(const char *parentDir, const char *projectName) {
  if (!parentDir || !projectName) {
    log_error("parentDir and projectName must be provided");
    return -1;
  }

  char *projectPath = constructPath(projectName, parentDir);
  if (!projectPath) {
    log_error("Failed to construct project path");
    return -1;
  }

  cdirnutsDir projectDir = {.path = projectPath,
                            .subDirCount = 0,
                            .subDirs = NULL,
                            .fileCount = 0,
                            .files = NULL};

  char *srcPath = constructPath("src", projectPath);
  char *includePath = constructPath("include", projectPath);
  char *testsPath = constructPath("tests", projectPath);

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
    free(projectPath);
    free(srcPath);
    free(includePath);
    free(testsPath);
    return -1;
  }

  char *readmePath = constructPath("README.md", projectPath);
  if (!readmePath) {
    log_error("Failed to construct README path");
    free(projectPath);
    return -1;
  }

  char *readmeContent = malloc(512);
  if (!readmeContent) {
    log_error("Failed to allocate memory for README content");
    free(readmePath);
    free(projectPath);
    return -1;
  }
  snprintf(readmeContent, 512, "# %s\n\nProject initialized by cdirnuts.\n",
           projectName);

  cdirnutsFile readmeFile = {.path = readmePath, .content = readmeContent};

  if (addFileToDir(&projectDir, &readmeFile) != 0) {
    log_error("Failed to add README to project");
    free(readmeContent);
    free(readmePath);
    free(projectPath);
    return -1;
  }

  char guard[256];
  size_t gi = 0;
  for (size_t i = 0; i < strlen(projectName) && gi + 1 < sizeof(guard); i++) {
    char c = projectName[i];
    if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
      guard[gi++] = (char)toupper(c);
    } else if (c >= 'A' && c <= 'Z') {
      guard[gi++] = c;
    } else {
      guard[gi++] = '_';
    }
  }
  guard[gi] = '_';
  guard[gi + 1] = 'H';
  guard[gi + 2] = '\0';

  char headerName[PATH_MAX];
  snprintf(headerName, sizeof(headerName), "%s.h", projectName);
  char *headerPath = constructPath(headerName, includePath);
  if (!headerPath) {
    log_error("Failed to construct header path");
    free(readmeContent);
    free(projectPath);
    return -1;
  }

  char *headerContent = malloc(512);
  if (!headerContent) {
    log_error("Failed to allocate memory for header content");
    free(headerPath);
    free(readmeContent);
    free(projectPath);
    return -1;
  }
  snprintf(
      headerContent, 512,
      "#ifndef %s\n#define %s\n\n// Public header for %s\n\n#endif // %s\n",
      guard, guard, projectName, guard);

  cdirnutsFile headerFile = {.path = headerPath, .content = headerContent};

  if (addFileToDir(&projectDir.subDirs[1], &headerFile) != 0) {
    log_error("Failed to add header to include directory");
    free(headerContent);
    free(headerPath);
    free(readmeContent);
    free(projectPath);
    return -1;
  }

  int result = createDir(&projectDir);

  free(headerContent);
  free(readmeContent);
  free(projectPath);

  if (result != 0) {
    log_error("Failed to create project structure");
    return -1;
  }

  log_info("Project %s initialized successfully at %s", projectName, parentDir);
  return 0;
}
