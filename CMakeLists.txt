cmake_minimum_required(VERSION 3.13)
project(cdirnuts C)

set(CMAKE_C_STANDARD 23)

add_executable(cdirnuts ${CMAKE_SOURCE_DIR}/src/main.c)

target_include_directories(cdirnuts PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_library(config_impl STATIC src/config_utils/config_utils.c)
# Match the compiler and linker flags from the Makefile

target_link_libraries(cdirnuts PRIVATE config_impl)
target_compile_options(cdirnuts PRIVATE -Wall -Wextra -Werror -fsanitize=address -g)
target_link_options(cdirnuts PRIVATE -fsanitize=address)

# Place the runtime binary in the repository's build/ directory so existing scripts continue to work
set_target_properties(cdirnuts PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

# Optional: a small option to build tests if desired
option(BUILD_TESTS "Build test executable" OFF)
if(BUILD_TESTS)
    add_executable(cdirnuts_test test/tests/test.c)
    target_include_directories(cdirnuts_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_options(cdirnuts_test PRIVATE -Wall -Wextra -Werror -fsanitize=address -g)
    target_link_options(cdirnuts_test PRIVATE -fsanitize=address)
    set_target_properties(cdirnuts_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
    enable_testing()
    add_test(NAME cdirnuts_test COMMAND ${CMAKE_SOURCE_DIR}/build/cdirnuts_test)
endif()
