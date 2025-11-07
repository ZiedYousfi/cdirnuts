# CMake script to embed a Lua file as a C string
# Usage: cmake -DLUA_FILE=<input> -DOUTPUT_FILE=<output> -P embed_lua.cmake

if(NOT DEFINED LUA_FILE OR NOT DEFINED OUTPUT_FILE)
    message(FATAL_ERROR "Usage: cmake -DLUA_FILE=<input> -DOUTPUT_FILE=<output> -P embed_lua.cmake")
endif()

# Read the Lua file
file(READ "${LUA_FILE}" LUA_CONTENT)

# Remove shebang line if present (#!/usr/bin/env lua or similar)
string(REGEX REPLACE "^#![^\n]*\n" "" LUA_CONTENT "${LUA_CONTENT}")

# Escape special characters for C string literal
string(REPLACE "\\" "\\\\" LUA_CONTENT "${LUA_CONTENT}")
string(REPLACE "\"" "\\\"" LUA_CONTENT "${LUA_CONTENT}")
string(REPLACE "\n" "\\n\"\n\"" LUA_CONTENT "${LUA_CONTENT}")

# Generate the C file
file(WRITE "${OUTPUT_FILE}"
"// Auto-generated file - DO NOT EDIT
// Generated from: ${LUA_FILE}

#include \"../include/default_lua_script.h\"

static const char *DEFAULT_LUA_SCRIPT =
\"${LUA_CONTENT}\";
")

message(STATUS "Generated ${OUTPUT_FILE} from ${LUA_FILE}")
