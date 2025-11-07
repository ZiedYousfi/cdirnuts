cmake_minimum_required(VERSION 3.13)
project(
  cdirnuts CXX
  VERSION 0.2.0
)

set(CMAKE_CXX_STANDARD 26)

# Generate embedded Lua script
set(EMBEDDED_LUA_SOURCE "${CMAKE_BINARY_DIR}/default_lua_script.c")
add_custom_command(
  OUTPUT "${EMBEDDED_LUA_SOURCE}"
  COMMAND ${CMAKE_COMMAND}
    -DLUA_FILE=${CMAKE_SOURCE_DIR}/default_init.lua
    -DOUTPUT_FILE=${EMBEDDED_LUA_SOURCE}
    -P ${CMAKE_SOURCE_DIR}/cmake/embed_lua.cmake
  DEPENDS "${CMAKE_SOURCE_DIR}/default_init.lua" "${CMAKE_SOURCE_DIR}/cmake/embed_lua.cmake"
  COMMENT "Embedding default_init.lua as C string"
  VERBATIM
)

target_include_directories(cdirnuts PRIVATE ${CMAKE_SOURCE_DIR}/include)
find_package(Lua REQUIRED)
target_link_libraries(cdirnuts PRIVATE Lua::Lua)
