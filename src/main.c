#include <limits.h>
#include <unistd.h>

#include "../include/dir.h"
#include "../include/init.h"
#include "../include/log.h"
#include "../include/lua_embed.h"
#include "../include/path.h"
#include "../include/preset.h"

/// @brief Prints help information to the console.
void print_help() {
  printf("Usage: cdirnuts [options]\n");
  printf("Options:\n");
  printf("  --help                  Show this help message\n");
  printf("  --config <file>         Specify a configuration file\n");
  printf("  --preset <name>         Use a preset by name\n");
  printf("  --preset list           List all saved presets\n");
  printf("  --preset add <name> <path>    Add a new preset\n");
  printf("  --preset remove <name>  Remove a preset by name\n");
  printf("If no project name is provided, 'my_project' will be used.\n");
}

typedef enum { OPT_HELP, OPT_CONFIG, OPT_PRESET } OptType;

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
 * - --preset <name>: loads a preset configuration by name (not yet
 * implemented).
 *
 * After parsing, the program logs a greeting and calls
 * init_default_setup(getcwd(NULL, 0), projectName) before performing cleanup.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @returns 0 on success, 1 if a required --config filename is missing or
 * invalid.
 */

int main(int argc, char **argv) {
  int result = 0;
  PathInfo *pathInfo = NULL;

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      OptType opt_type = 0;
      if (strcmp(argv[i], "--help") == 0) {
        opt_type = OPT_HELP;
      } else if (strcmp(argv[i], "--config") == 0) {
        opt_type = OPT_CONFIG;
      } else if (strcmp(argv[i], "--preset") == 0) {
        opt_type = OPT_PRESET;
      }

      switch (opt_type) {
      case OPT_HELP:
        print_help();
        result = 0;
        goto cleanup;

      case OPT_CONFIG:
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

        const char *luaScript = argv[i + 1];
        log_info("Executing Lua script: %s", luaScript);

        result = execute_lua_script(luaScript);
        goto cleanup;

      case OPT_PRESET:
        if (i + 1 >= argc) {
          log_error("Error: --preset option requires an argument.");
          result = 1;
          goto cleanup;
        }

        const char *subcommand = argv[i + 1];

        if (strcmp(subcommand, "list") == 0) {
          listPresets();
          result = 0;
          goto cleanup;
        } else if (strcmp(subcommand, "add") == 0) {
          if (i + 3 >= argc) {
            log_error(
                "Error: --preset add requires <name> and <path> arguments.");
            result = 1;
            goto cleanup;
          }
          const char *presetName = argv[i + 2];
          const char *presetPath = argv[i + 3];

          int addResult = addPreset(presetName, presetPath);
          if (addResult == 0) {
            log_info("Preset '%s' added successfully.", presetName);
            result = 0;
          } else if (addResult == -2) {
            log_error("Error: Preset '%s' already exists.", presetName);
            result = 1;
          } else {
            log_error("Error: Failed to add preset '%s'.", presetName);
            result = 1;
          }
          goto cleanup;
        } else if (strcmp(subcommand, "remove") == 0) {
          if (i + 2 >= argc) {
            log_error("Error: --preset remove requires <name> argument.");
            result = 1;
            goto cleanup;
          }
          const char *presetName = argv[i + 2];

          int removeResult = removePreset(presetName);
          if (removeResult == 0) {
            log_info("Preset '%s' removed successfully.", presetName);
            result = 0;
          } else if (removeResult == -2) {
            log_error("Error: Preset '%s' not found.", presetName);
            result = 1;
          } else {
            log_error("Error: Failed to remove preset '%s'.", presetName);
            result = 1;
          }
          goto cleanup;
        } else {
          // Treat as preset name to use directly
          const char *presetName = subcommand;

          // Find the preset
          Preset *presets = getPresets();
          if (!presets) {
            log_error("Error: No presets found.");
            result = 1;
            goto cleanup;
          }

          int found = 0;
          int count = 0;
          while (presets[count].name != NULL) {
            if (strcmp(presets[count].name, presetName) == 0) {
              found = 1;
              log_info("Using preset '%s' with path: %s", presetName,
                       presets[count].path);
              result = execute_lua_script(presets[count].path);
              break;
            }
            count++;
          }

          // Free memory
          for (int j = 0; j < count; j++) {
            free(presets[j].name);
            free(presets[j].path);
          }
          free(presets);

          if (!found) {
            log_error("Error: Preset '%s' not found.", presetName);
            result = 1;
          }
          goto cleanup;
        }
      }
    }
  } else {
    result = init_default_setup();
    goto cleanup;
  }
cleanup:
  if (pathInfo)
    free(pathInfo);
  return result;
}
