cmake_minimum_required(VERSION 3.13)
project(cdirnuts C)

set(CMAKE_C_STANDARD 23)

# Trouver Lua via vcpkg (manifeste vcpkg.json contient "lua")
find_package(Lua REQUIRED)

# Some Lua packages (depending on how vcpkg / system packages are provided)
# export an imported target (Lua::Lua). The old FindLua module provides
# variables such as LUA_LIBRARIES and LUA_INCLUDE_DIR. Handle both cases so
# the project works regardless of how Lua was found.
set(_USE_LUA_TARGET OFF)
if(TARGET Lua::Lua)
  set(_USE_LUA_TARGET ON)
endif()

add_executable(cdirnuts ${CMAKE_SOURCE_DIR}/src/main.c)
target_include_directories(cdirnuts PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_library(config_impl STATIC src/config.c)
add_library(dir_impl STATIC src/dir.c)
add_library(path_impl STATIC src/path.c)
add_library(init_impl STATIC src/init.c)
add_library(log_impl STATIC src/log.c)
add_library(lua_impl STATIC src/lua_embed.c)

target_link_libraries(init_impl PRIVATE dir_impl path_impl log_impl)
target_link_libraries(dir_impl PRIVATE path_impl log_impl)
target_link_libraries(config_impl PRIVATE log_impl)

# Si seul main.c utilise l'API Lua:
if(_USE_LUA_TARGET)
  target_link_libraries(cdirnuts PRIVATE config_impl dir_impl path_impl init_impl log_impl Lua::Lua lua_impl)
else()
  target_include_directories(cdirnuts PRIVATE ${LUA_INCLUDE_DIR})
  target_link_libraries(cdirnuts PRIVATE config_impl dir_impl path_impl init_impl log_impl ${LUA_LIBRARIES})
endif()

# Si d'autres libs appellent l'API Lua (ex: init_impl):
# target_link_libraries(init_impl PRIVATE Lua::Lua)

target_compile_options(cdirnuts PRIVATE -Wall -Wextra -Werror -fsanitize=address -g)
target_link_options(cdirnuts PRIVATE -fsanitize=address)

set_target_properties(cdirnuts PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

option(BUILD_TESTS "Build test executable" OFF)
if(BUILD_TESTS)
    add_executable(cdirnuts_test test/tests/test.c)
    target_include_directories(cdirnuts_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_options(cdirnuts_test PRIVATE -Wall -Wextra -Werror -fsanitize=address -g)
    target_link_options(cdirnuts_test PRIVATE -fsanitize=address)
    if(_USE_LUA_TARGET)
      target_link_libraries(cdirnuts_test PRIVATE Lua::Lua)
    else()
      target_include_directories(cdirnuts_test PRIVATE ${LUA_INCLUDE_DIR})
      target_link_libraries(cdirnuts_test PRIVATE ${LUA_LIBRARIES})
    endif()
    set_target_properties(cdirnuts_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
    enable_testing()
    add_test(NAME cdirnuts_test COMMAND ${CMAKE_SOURCE_DIR}/build/cdirnuts_test)
endif()
