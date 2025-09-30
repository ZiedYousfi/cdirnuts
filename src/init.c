#include "../include/init.h"
#include "../include/path.h"

static int create_dir_if_not_exists(const char *path) {
  if (!path) {
    log_error("create_dir_if_not_exists: path is NULL");
    return -1;
  }

  struct stat st;
  if (stat(path, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      log_info("Directory %s already exists", path);
      return 0;
    } else {
      log_error("Path %s exists but is not a directory", path);
      return -1;
    }
  }

  if (mkdir(path, 0755) != 0) {
    log_error("Failed to create directory %s: %s", path, strerror(errno));
    return -1;
  }

  log_info("Directory %s created", path);
  return 0;
}

static int create_file_if_not_exists(const char *path, const char *content) {
  if (!path) {
    log_error("create_file_if_not_exists: path is NULL");
    return -1;
  }

  FILE *f = fopen(path, "r");
  if (f) {
    fclose(f);
    log_info("File %s already exists", path);
    return 0;
  }

  f = fopen(path, "w");
  if (!f) {
    log_error("Failed to create file %s: %s", path, strerror(errno));
    return -1;
  }

  if (content) {
    size_t len = strlen(content);
    if (fwrite(content, 1, len, f) < len) {
      log_error("Failed to write to file %s", path);
      fclose(f);
      return -1;
    }
  }

  fclose(f);
  log_info("File %s created", path);
  return 0;
}

int init_default_setup(const char *parentDir, const char *projectName) {
  if (!parentDir || !projectName) {
    log_error("parentDir and projectName must be provided");
    return -1;
  }

  int status = 0;

  char *projectPath = constructPath(projectName, parentDir);
  if (!projectPath) {
    log_error("Failed to construct project path");
    return -1;
  }

  // create main project directory
  if (create_dir_if_not_exists(projectPath) != 0) {
    free(projectPath);
    return -1;
  }

  // common subdirectories
  const char *subdirs[] = {"src", "include", "tests"};
  for (size_t i = 0; i < sizeof(subdirs) / sizeof(subdirs[0]); i++) {
    char *p = constructPath(subdirs[i], projectPath);
    if (!p) {
      status = -1;
      break;
    }
    if (create_dir_if_not_exists(p) != 0) {
      free(p);
      status = -1;
      break;
    }
    free(p);
  }

  if (status != 0) {
    free(projectPath);
    return -1;
  }

  // README
  char *readmePath = constructPath("README.md", projectPath);
  if (!readmePath) {
    free(projectPath);
    return -1;
  }
  char readmeContent[512];
  snprintf(readmeContent, sizeof(readmeContent),
           "# %s\n\nProject initialized by cdirnuts.\n", projectName);
  if (create_file_if_not_exists(readmePath, readmeContent) != 0) {
    free(readmePath);
    free(projectPath);
    return -1;
  }
  free(readmePath);

  // include header
  // create a safe header guard name from projectName
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

  char includeName[PATH_MAX];
  snprintf(includeName, sizeof(includeName), "%s.h", projectName);
  char *includePath = constructPath(includeName, projectPath);
  if (includePath) {
    // place inside include/ directory
    char *includeDirPath = constructPath("include", projectPath);
    if (includeDirPath) {
      char *fullInclude = constructPath(includeName, includeDirPath);
      if (fullInclude) {
        char includeContent[512];
        snprintf(includeContent, sizeof(includeContent),
                 "#ifndef %s\n#define %s\n\n// Public header for %s\n\n#endif "
                 "// %s\n",
                 guard, guard, projectName, guard);
        create_file_if_not_exists(fullInclude, includeContent);
        free(fullInclude);
      }
      free(includeDirPath);
    }
    free(includePath);
  }

  free(projectPath);
  return 0;
}
