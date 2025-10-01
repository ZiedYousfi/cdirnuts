#include <limits.h>
#include <unistd.h>

#include "../include/dir.h"
#include "../include/init.h"
#include "../include/log.h"
#include "../include/lua_embed.h"
#include "../include/path.h"

#define PROJECT_NAME "my_project"

/// @brief Prints help information to the console.
void print_help() {
  printf("Usage: cdirnuts [options] [project_name]\n");
  printf("Options:\n");
  printf("  --help            Show this help message\n");
  printf("  --config <file>   Specify a configuration file\n");
  printf("  --lua <script>    Execute a Lua script with cdirnuts API\n");
  printf("If no project name is provided, 'my_project' will be used.\n");
}

typedef enum { OPT_HELP, OPT_CONFIG, OPT_LUA, OPT_PROJECT_NAME } OptType;

/**
 * Program entry point that parses command-line options, performs default
 * initialization, and returns an exit status.
 *
 * Supported options:
 * - --help: prints usage and exits.
 * - --config <file>: loads configuration from <file>; if the file argument is
 * missing or begins with "--", the program logs an error and exits with
 * status 1. A single non-option argument is treated as the project name; if
 * none is provided, PROJECT_NAME is used.
 *
 * After parsing, the program logs a greeting and calls
 * init_default_setup(getcwd(NULL, 0), projectName) before performing cleanup.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @returns 0 on success, 1 if a required --config filename is missing or
 * invalid.
 */

int main(int argc, char *argv[]) {
  run_lua_smoke_test();
  int result = 0;
  bool shouldFreeProjectName = false;
  char *projectName = PROJECT_NAME;
  PathInfo *pathInfo = NULL;

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      OptType opt_type = 0;
      if (strcmp(argv[i], "--help") == 0) {
        opt_type = OPT_HELP;
      } else if (strcmp(argv[i], "--config") == 0) {
        opt_type = OPT_CONFIG;
      } else if (strcmp(argv[i], "--lua") == 0) {
        opt_type = OPT_LUA;
      } else {
        opt_type = OPT_PROJECT_NAME;
      }

      switch (opt_type) {
      case OPT_HELP:
        print_help();
        result = 0;
        goto cleanup;

      case OPT_CONFIG: {
        if (i + 1 >= argc) {
          log_error("Error: --config option requires a file argument.");
          result = 1;
          goto cleanup;
        }

        if (strncmp(argv[i + 1], "--", 2) == 0) {
          log_error("Error: Invalid config file name.");
          result = 1;
          goto cleanup;
        }

        const char *configFile = argv[i + 1];

        // Load configuration from the specified file
        // (Implementation of config loading is not yet done)

        log_info("Loading configuration from %s", configFile);
        i++;
        break;
      }
      case OPT_LUA: {
        if (i + 1 >= argc) {
          log_error("Error: --lua option requires a script file argument.");
          result = 1;
          goto cleanup;
        }

        if (strncmp(argv[i + 1], "--", 2) == 0) {
          log_error("Error: Invalid Lua script file name.");
          result = 1;
          goto cleanup;
        }

        const char *luaScript = argv[i + 1];
        log_info("Executing Lua script: %s", luaScript);

        if (execute_lua_script(luaScript) != 0) {
          log_error("Failed to execute Lua script");
          result = 1;
          goto cleanup;
        }

        result = 0;
        goto cleanup;
      }
      case OPT_PROJECT_NAME:
        projectName = argv[i];
        shouldFreeProjectName = false;
        break;
      }
    }
  }

  log_info("Hello, World!");
  init_default_setup(getcwd(NULL, 0), projectName);

cleanup:
  if (pathInfo)
    free(pathInfo);
  if (shouldFreeProjectName)
    free(projectName);
  return result;
}
