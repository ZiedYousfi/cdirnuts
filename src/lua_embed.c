#include "../include/lua_embed.h"
#include "../include/config.h"
#include "../include/dir.h"
#include "../include/log.h"
#include "../include/path.h"
#include <string.h>

// Metatable names for userdata types
#define CDIRNUTS_DIR_MT "cdirnuts.Dir"
#define CDIRNUTS_FILE_MT "cdirnuts.File"
#define CDIRNUTS_PATHINFO_MT "cdirnuts.PathInfo"

// Helper function to check if a userdata is a directory
static cdirnutsDir **check_dir(lua_State *L, int index) {
  return (cdirnutsDir **)luaL_checkudata(L, index, CDIRNUTS_DIR_MT);
}

// Helper function to check if a userdata is a file
static cdirnutsFile **check_file(lua_State *L, int index) {
  return (cdirnutsFile **)luaL_checkudata(L, index, CDIRNUTS_FILE_MT);
}

// Helper function to check if a userdata is a PathInfo
static PathInfo **check_pathinfo(lua_State *L, int index) {
  return (PathInfo **)luaL_checkudata(L, index, CDIRNUTS_PATHINFO_MT);
}

// ============================================================================
// Utility functions
// ============================================================================
char *getCWD(void) { return getcwd(NULL, 0); }

static int l_getCWD(lua_State *L) {
  char *cwd = getCWD();
  if (cwd) {
    lua_pushstring(L, cwd);
    free(cwd);
  } else {
    lua_pushnil(L);
    lua_pushstring(L, "Failed to get current working directory");
    return 2;
  }
  return 1;
}
// ============================================================================
// Directory functions
// ============================================================================

/**
 * Lua: dir = cdirnuts.allocDir(path)
 * Allocates a new directory object.
 */
static int l_allocDir(lua_State *L) {
  const char *path = luaL_checkstring(L, 1);

  cdirnutsDir *dir = allocDir(path);
  if (!dir) {
    lua_pushnil(L);
    lua_pushstring(L, "Failed to allocate directory");
    return 2;
  }

  // Create userdata to hold the directory pointer
  cdirnutsDir **udata =
      (cdirnutsDir **)lua_newuserdata(L, sizeof(cdirnutsDir *));
  *udata = dir;

  luaL_getmetatable(L, CDIRNUTS_DIR_MT);
  lua_setmetatable(L, -2);

  return 1;
}

/**
 * Lua: success = cdirnuts.createDir(dir)
 * Creates a directory and all its subdirectories/files.
 */
static int l_createDir(lua_State *L) {
  cdirnutsDir **dir = check_dir(L, 1);

  if (!*dir) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "Invalid directory object");
    return 2;
  }

  int result = createDir(*dir);
  lua_pushboolean(L, result == 0);

  if (result != 0) {
    lua_pushstring(L, "Failed to create directory");
    return 2;
  }

  return 1;
}

/**
 * Lua: success = cdirnuts.addSubDirToDir(parentDir, childDir)
 * Adds a subdirectory to a parent directory (transfers ownership).
 */
static int l_addSubDirToDir(lua_State *L) {
  cdirnutsDir **parent = check_dir(L, 1);
  cdirnutsDir **child = check_dir(L, 2);

  if (!*parent || !*child) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "Invalid directory object");
    return 2;
  }

  int result = addSubDirToDir(*parent, *child);

  // After transfer, the child pointer is cleared by addSubDirToDir
  // We need to mark it as transferred to prevent double-free
  *child = NULL;

  lua_pushboolean(L, result == 0);

  if (result != 0) {
    lua_pushstring(L, "Failed to add subdirectory");
    return 2;
  }

  return 1;
}

/**
 * Lua: file = cdirnuts.createFile(path, content)
 * Creates a file object (not written to disk yet).
 */
static int l_createFileObj(lua_State *L) {
  const char *path = luaL_checkstring(L, 1);
  const char *content = lua_isnil(L, 2) ? NULL : luaL_checkstring(L, 2);

  cdirnutsFile **udata =
      (cdirnutsFile **)lua_newuserdata(L, sizeof(cdirnutsFile *));
  cdirnutsFile *file = (cdirnutsFile *)malloc(sizeof(cdirnutsFile));

  if (!file) {
    lua_pushnil(L);
    lua_pushstring(L, "Failed to allocate file");
    return 2;
  }

  file->path = strdup(path);
  file->content = content ? strdup(content) : NULL;

  *udata = file;

  luaL_getmetatable(L, CDIRNUTS_FILE_MT);
  lua_setmetatable(L, -2);

  return 1;
}

/**
 * Lua: success = cdirnuts.writeFile(file)
 * Writes a file object to disk.
 */
static int l_writeFile(lua_State *L) {
  cdirnutsFile **file = check_file(L, 1);

  if (!*file) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "Invalid file object");
    return 2;
  }

  int result = createFile(*file);
  lua_pushboolean(L, result == 0);

  if (result != 0) {
    lua_pushstring(L, "Failed to write file");
    return 2;
  }

  return 1;
}

/**
 * Lua: success = cdirnuts.addFileToDir(dir, file)
 * Adds a file to a directory (deep copy).
 */
static int l_addFileToDir(lua_State *L) {
  cdirnutsDir **dir = check_dir(L, 1);
  cdirnutsFile **file = check_file(L, 2);

  if (!*dir || !*file) {
    lua_pushboolean(L, 0);
    lua_pushstring(L, "Invalid directory or file object");
    return 2;
  }

  int result = addFileToDir(*dir, *file);
  lua_pushboolean(L, result == 0);

  if (result != 0) {
    lua_pushstring(L, "Failed to add file to directory");
    return 2;
  }

  return 1;
}

/**
 * Garbage collector for directory objects.
 */
static int l_freeDir(lua_State *L) {
  cdirnutsDir **dir = check_dir(L, 1);

  if (*dir) {
    freeDir(*dir);
    *dir = NULL;
  }

  return 0;
}

/**
 * Garbage collector for file objects.
 */
static int l_freeFile(lua_State *L) {
  cdirnutsFile **file = check_file(L, 1);

  if (*file) {
    free((*file)->path);
    free((*file)->content);
    free(*file);
    *file = NULL;
  }

  return 0;
}

// ============================================================================
// Config/Command functions
// ============================================================================

/**
 * Lua: success = cdirnuts.executeCommand(command)
 * Executes a shell command.
 */
static int l_executeCommand(lua_State *L) {
  const char *command = luaL_checkstring(L, 1);

  int result = executeCommand(command);
  lua_pushboolean(L, result == 0);

  if (result != 0) {
    lua_pushstring(L, "Command execution failed");
    return 2;
  }

  return 1;
}

// ============================================================================
// Registration
// ============================================================================

void register_cdirnuts_lua_api(lua_State *L) {
  // Create Dir metatable
  luaL_newmetatable(L, CDIRNUTS_DIR_MT);
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, l_freeDir);
  lua_settable(L, -3);
  lua_pop(L, 1);

  // Create File metatable
  luaL_newmetatable(L, CDIRNUTS_FILE_MT);
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, l_freeFile);
  lua_settable(L, -3);
  lua_pop(L, 1);

  // Create cdirnuts table
  lua_newtable(L);

  // Utility functions
  lua_pushstring(L, "getCWD");
  lua_pushcfunction(L, l_getCWD);
  lua_settable(L, -3);

  // Directory functions
  lua_pushstring(L, "allocDir");
  lua_pushcfunction(L, l_allocDir);
  lua_settable(L, -3);

  lua_pushstring(L, "createDir");
  lua_pushcfunction(L, l_createDir);
  lua_settable(L, -3);

  lua_pushstring(L, "addSubDirToDir");
  lua_pushcfunction(L, l_addSubDirToDir);
  lua_settable(L, -3);

  // File functions
  lua_pushstring(L, "createFile");
  lua_pushcfunction(L, l_createFileObj);
  lua_settable(L, -3);

  lua_pushstring(L, "writeFile");
  lua_pushcfunction(L, l_writeFile);
  lua_settable(L, -3);

  lua_pushstring(L, "addFileToDir");
  lua_pushcfunction(L, l_addFileToDir);
  lua_settable(L, -3);

  // Command functions
  lua_pushstring(L, "executeCommand");
  lua_pushcfunction(L, l_executeCommand);
  lua_settable(L, -3);

  // Set the table as global "cdirnuts"
  lua_setglobal(L, "cdirnuts");
}

int execute_lua_script(const char *scriptPath) {
  if (!scriptPath) {
    log_error("Script path is NULL");
    return -1;
  }

  lua_State *L = luaL_newstate();
  if (!L) {
    log_error("Failed to create Lua state");
    return -1;
  }

  luaL_openlibs(L);
  register_cdirnuts_lua_api(L);

  if (luaL_dofile(L, scriptPath) != LUA_OK) {
    log_error("Lua error: %s", lua_tostring(L, -1));
    lua_pop(L, 1);
    lua_close(L);
    return -1;
  }

  lua_close(L);
  log_info("Successfully executed Lua script: %s", scriptPath);
  return 0;
}

int execute_lua_string(const char *luaCode) {
  if (!luaCode) {
    log_error("Lua code string is NULL");
    return -1;
  }

  lua_State *L = luaL_newstate();
  if (!L) {
    log_error("Failed to create Lua state");
    return -1;
  }

  luaL_openlibs(L);
  register_cdirnuts_lua_api(L);

  if (luaL_dostring(L, luaCode) != LUA_OK) {
    log_error("Lua error: %s", lua_tostring(L, -1));
    lua_pop(L, 1);
    lua_close(L);
    return -1;
  }

  lua_close(L);
  log_info("Successfully executed Lua code from string");
  return 0;
}

void run_lua_smoke_test(void) {
  lua_State *L = luaL_newstate();
  if (!L) {
    fprintf(stderr, "Failed to create Lua state\n");
    return;
  }
  luaL_openlibs(L);

  const char *script = "print('Lua OK: 2+3=', 2+3)\n"
                       "function add(a,b) return a+b end";

  if (luaL_dostring(L, script) != LUA_OK) {
    fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
    lua_close(L);
    return;
  }

  lua_getglobal(L, "add");
  lua_pushnumber(L, 10);
  lua_pushnumber(L, 32);
  if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
    fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  } else {
    double res = lua_tonumber(L, -1);
    printf("add(10,32) = %.0f\n", res);
    lua_pop(L, 1);
  }

  lua_close(L);
}
