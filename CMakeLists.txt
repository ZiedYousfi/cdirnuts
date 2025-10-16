cmake_minimum_required(VERSION 3.13)
project(cdirnuts C)

set(CMAKE_C_STANDARD 23)

find_package(Lua REQUIRED)
# Configure Lua usage: prefer imported target Lua::Lua, otherwise fall back to include/libs
if(TARGET Lua::Lua)
  set(_USE_LUA_TARGET TRUE)
else()
  set(_USE_LUA_TARGET FALSE)
  if(NOT DEFINED LUA_INCLUDE_DIR OR NOT LUA_INCLUDE_DIR)
    find_path(LUA_INCLUDE_DIR NAMES lua.h PATH_SUFFIXES lua5.4 lua5.3 lua5.2)
  endif()
  if(NOT DEFINED LUA_LIBRARIES OR NOT LUA_LIBRARIES)
    find_library(LUA_LIBRARIES NAMES lua5.4 lua5.3 lua5.2 lua)
  endif()
endif()

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

add_executable(cdirnuts ${CMAKE_SOURCE_DIR}/src/main.c ${EMBEDDED_LUA_SOURCE})
target_include_directories(cdirnuts PRIVATE ${CMAKE_SOURCE_DIR}/include ${LUA_INCLUDE_DIR})

add_library(config_impl STATIC src/config.c)
add_library(dir_impl STATIC src/dir.c)
add_library(path_impl STATIC src/path.c)
add_library(init_impl STATIC src/init.c ${EMBEDDED_LUA_SOURCE})
add_library(log_impl STATIC src/log.c)
add_library(lua_impl STATIC src/lua_embed.c)
add_library(preset_impl STATIC src/preset.c)

target_link_libraries(init_impl PUBLIC dir_impl path_impl log_impl lua_impl)
target_link_libraries(dir_impl PUBLIC path_impl log_impl)
target_link_libraries(config_impl PUBLIC log_impl)
target_link_libraries(preset_impl PUBLIC log_impl)
target_include_directories(init_impl PRIVATE ${LUA_INCLUDE_DIR})

if(_USE_LUA_TARGET)
  # lua_impl depends on log_impl and config_impl (executeCommand is in config_impl)
  target_link_libraries(lua_impl PRIVATE Lua::Lua m log_impl config_impl)
else()
  target_include_directories(lua_impl PRIVATE ${LUA_INCLUDE_DIR})
  # link config_impl so symbols like executeCommand are available
  target_link_libraries(lua_impl PRIVATE ${LUA_LIBRARIES} m log_impl config_impl)
endif()
target_include_directories(cdirnuts PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(cdirnuts PRIVATE config_impl init_impl lua_impl preset_impl)

target_compile_options(cdirnuts PRIVATE -Wall -Wextra -Werror -fsanitize=address -g)
target_link_options(cdirnuts PRIVATE -fsanitize=address)

set_target_properties(cdirnuts PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

option(BUILD_TESTS "Build test executable" OFF)
if(BUILD_TESTS)
    add_executable(cdirnuts_test ${CMAKE_SOURCE_DIR}/tests/test.c)
    target_include_directories(cdirnuts_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_options(cdirnuts_test PRIVATE -Wall -Wextra -Werror -fsanitize=address -g)
    target_link_options(cdirnuts_test PRIVATE -fsanitize=address)
    if(_USE_LUA_TARGET)
      # Link the project's implementation libraries so tests can resolve
      # symbols such as fromString, toString, getPresets, savePresets and
      # the logging functions.
      target_link_libraries(cdirnuts_test PRIVATE Lua::Lua m preset_impl config_impl log_impl dir_impl path_impl)
    else()
      target_include_directories(cdirnuts_test PRIVATE ${LUA_INCLUDE_DIR})
      target_link_libraries(cdirnuts_test PRIVATE ${LUA_LIBRARIES} m preset_impl config_impl log_impl dir_impl path_impl)
    endif()
    set_target_properties(cdirnuts_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
    enable_testing()
    add_test(NAME cdirnuts_test COMMAND ${CMAKE_SOURCE_DIR}/build/cdirnuts_test)
endif()
