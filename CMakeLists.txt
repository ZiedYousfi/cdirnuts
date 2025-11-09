################################################################################
# CMake Configuration for cdirnuts
################################################################################

cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(
  cdirnuts
  LANGUAGES CXX C
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

# Enable IPO/LTO if supported (cross-platform way)
# Disable on Windows MinGW due to linking issues with vcpkg libraries
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
if(MINGW)
  set(ipo_supported FALSE)
  message(STATUS "IPO/LTO disabled on MinGW due to vcpkg library compatibility")
elseif(ipo_supported)
  message(STATUS "IPO/LTO is supported and will be enabled for Release builds")
else()
  message(STATUS "IPO/LTO is not supported: ${ipo_output}")
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
  ${EMBEDDED_LUA_SOURCE}
)

# Set standard properties
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  SOVERSION 0
  VERSION ${PROJECT_VERSION}
  INTERPROCEDURAL_OPTIMIZATION_RELEASE ${ipo_supported}
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
    ${LUA_LIBRARIES}
    CLI11::CLI11
    sol2::sol2
)

################################################################################
# Executable Target
################################################################################

add_executable(${PROJECT_NAME}_exec
  src/main.cpp
)

set_target_properties(${PROJECT_NAME}_exec PROPERTIES
  OUTPUT_NAME ${PROJECT_NAME}
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION_RELEASE ${ipo_supported}
)

target_link_libraries(${PROJECT_NAME}_exec
  PRIVATE
    ${PROJECT_NAME}
    CLI11::CLI11
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

################################################################################
# Testing
################################################################################

if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)

  # Test sources
  set(TEST_SOURCES
    tests/test_fs.cpp
    tests/test_presets.cpp
    tests/test_lua.cpp
  )

  # Create test executable
  add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

  set_target_properties(${PROJECT_NAME}_tests PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )

  target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
      ${PROJECT_NAME}
      GTest::gtest
      GTest::gtest_main
      GTest::gmock
  )

  target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  # Discover tests only if not cross-compiling
  # (cross-compilation would fail because the test executable is for a different architecture)
  if(NOT CMAKE_CROSSCOMPILING)
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_tests)
  endif()
endif()
