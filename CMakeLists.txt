################################################################################
# CMake Configuration for cdirnuts
################################################################################

cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(
  cdirnuts
  LANGUAGES CXX
  VERSION 0.2.0
  DESCRIPTION "A Lua-scriptable CLI application"
  HOMEPAGE_URL "https://github.com/ZiedYousfi/cdirnuts"
)

################################################################################
# Project Settings
################################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable reproducible builds
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# Build Configuration
################################################################################

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Compiler flags for all configurations
if(MSVC)
  # MSVC-specific flags
  add_compile_options(/W4 /permissive-)
else()
  # GCC/Clang flags
  add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
endif()

################################################################################
# External Dependencies
################################################################################

find_package(Lua REQUIRED)
find_package(CLI11 REQUIRED)
find_package(sol2 REQUIRED)

################################################################################
# Generate Embedded Lua Script
################################################################################

set(EMBEDDED_LUA_SOURCE "${CMAKE_BINARY_DIR}/default_lua_script.c")
add_custom_command(
  OUTPUT "${EMBEDDED_LUA_SOURCE}"
  COMMAND ${CMAKE_COMMAND}
    -DLUA_FILE=${CMAKE_SOURCE_DIR}/default_init.lua
    -DOUTPUT_FILE=${EMBEDDED_LUA_SOURCE}
    -P ${CMAKE_SOURCE_DIR}/cmake/embed_lua.cmake
  DEPENDS
    "${CMAKE_SOURCE_DIR}/default_init.lua"
    "${CMAKE_SOURCE_DIR}/cmake/embed_lua.cmake"
  COMMENT "Generating embedded Lua script"
  VERBATIM
)

################################################################################
# Source Files
################################################################################

# Library sources
set(${PROJECT_NAME}_SOURCES
  src/fs.cpp
  src/presets.cpp
  src/lua.cpp
)

# Library headers
set(${PROJECT_NAME}_HEADERS
  include/fs.h
  include/presets.h
  include/lua.h
)

################################################################################
# Main Library Target
################################################################################

add_library(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
)

# Set standard properties
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  SOVERSION 0
  VERSION ${PROJECT_VERSION}
)

# Compiler options (cross-platform)
target_compile_options(${PROJECT_NAME}
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

# Include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Lua::Lua
    CLI11::CLI11
    sol2::sol2
)

################################################################################
# Executable Target
################################################################################

add_executable(${PROJECT_NAME}_exec
  src/main.cpp
  ${EMBEDDED_LUA_SOURCE}
)

set_target_properties(${PROJECT_NAME}_exec PROPERTIES
  OUTPUT_NAME ${PROJECT_NAME}
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_link_libraries(${PROJECT_NAME}_exec
  PRIVATE
    ${PROJECT_NAME}
)

################################################################################
# Installation
################################################################################

include(GNUInstallDirs)

install(
  TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_exec
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

################################################################################
# Warnings and Best Practices
################################################################################

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wconversion -Wsign-conversion)
endif()

# Ensure proper symbol visibility
if(NOT MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN ON)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
endif()
