cmake_minimum_required(VERSION 3.13)
project(cdirnuts C)

set(CMAKE_C_STANDARD 23)

find_package(Lua REQUIRED)

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

target_link_libraries(lua_impl PRIVATE Lua::Lua log_impl)
target_include_directories(cdirnuts PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(cdirnuts PRIVATE config_impl dir_impl path_impl init_impl log_impl lua_impl)

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
