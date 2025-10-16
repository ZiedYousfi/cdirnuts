#pragma once

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stddef.h>
#include <stdint.h>

void run_lua_smoke_test(void);

/**
 * @brief Registers all cdirnuts functions into the Lua state.
 *
 * This function creates a global table "cdirnuts" in Lua with all
 * the exposed C functions for directory and file manipulation.
 *
 * @param L The Lua state to register functions into.
 */
void register_cdirnuts_lua_api(lua_State *L);

/**
 * @brief Executes a Lua script file with the cdirnuts API available.
 *
 * @param scriptPath Path to the Lua script file to execute.
 * @return 0 on success, -1 on failure.
 */
int execute_lua_script(const char *scriptPath);

/**
 * @brief Executes Lua code from a string with the cdirnuts API available.
 *
 * @param luaCode The Lua code to execute as a string.
 * @return 0 on success, -1 on failure.
 */
int execute_lua_string(const char *luaCode);

// Lua utility functions

char *getCWD(void);
