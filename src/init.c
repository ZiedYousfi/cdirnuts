#include "../include/init.h"
#include "../include/default_lua_script.h"
#include "../include/lua_embed.h"
#include "../include/path.h"

int init_default_setup(const char *parentDir, const char *projectName) {
  if (!parentDir || !projectName) {
    log_error("Invalid arguments: parentDir and projectName must not be NULL");
    return -1;
  }

  log_info("Initializing default setup for project '%s' in directory '%s'",
           projectName, parentDir);

  // Execute the embedded Lua script directly from the compile-time constant
  int result = execute_lua_string(DEFAULT_LUA_SCRIPT);

  if (result != 0) {
    log_error("Failed to execute default setup script");
    return -1;
  }

  log_info("Default setup completed successfully");
  return 0;
}
