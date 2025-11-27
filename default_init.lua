#!/usr/bin/env lua

-- Example Lua script demonstrating the cdirnuts API
-- This script creates a complete project structure with directories and files

print("=== cdirnuts Lua API Example ===\n")

-- The cdirnuts API is already registered globally when embedded
-- No need to require it

-- Get the current working directory
local cwd = cdirnuts.getCWD()

-- Ask user for the project name
print("1. Project Setup:")
print("   --------------")
io.write("Enter the project name (default: test_project): ")
local projectName = io.read()
if projectName == nil or projectName == "" then
    projectName = "test_project"
end
print("   ✓ Project name set to: " .. projectName)

-- Ask user for the programming language
io.write("Choose language (c/cpp, default: c): ")
local language = io.read()
if language == nil or language == "" then
    language = "c"
end
language = string.lower(language)
if language ~= "c" and language ~= "cpp" then
    language = "c"
end
local ext = (language == "cpp") and "cpp" or "c"
print("   ✓ Language set to: " .. language .. " (extension: " .. ext .. ")")

print("\n2. Creating Project Structure:")
print("   ---------------------------")

-- Create root directory
local rootDir = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName)
print("   ✓ Created root directory: ./" .. projectName)

-- Create src directory
local srcDir = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName .. "/src")
print("   ✓ Created src directory")

-- Create include directory
local includeDir = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName .. "/include")
print("   ✓ Created include directory")

-- Create tests directory
local testsDir = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName .. "/tests")
print("   ✓ Created tests directory")

-- Create files
print("\n3. Creating Files:")
print("   ---------------")

-- Create README.md
local readmeFile = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/README.md",
    [[# ]] .. projectName .. [[

This project was created using the cdirnuts Lua API!

## Features
- Automatic project scaffolding
- Lua-based configuration
- Easy to extend
]]
)
print("   ✓ Created README.md file object")

-- Create main.c or main.cpp
local mainFileName = "main." .. ext
local mainContent
if language == "cpp" then
    mainContent = [[#include <iostream>

int main(int argc, char **argv) {
    std::cout << "Hello from ]] .. projectName .. [[!" << std::endl;
    return 0;
}
]]
else
    mainContent = [[#include <stdio.h>

int main(int argc, char **argv) {
    printf("Hello from ]] .. projectName .. [[!\n");
    return 0;
}
]]
end

local mainFile = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/src/" .. mainFileName,
    mainContent
)
print("   ✓ Created " .. mainFileName .. " file object")

-- Create header file
local headerFile = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/include/app.h",
    [[#pragma once

void app_init(void);
void app_run(void);
void app_cleanup(void);
]]
)
print("   ✓ Created app.h file object")

-- Create test file
local testFileName = "test_main." .. ext
local testContent
if language == "cpp" then
    testContent = [[#include <cassert>
#include <iostream>

void test_basic() {
    assert(1 + 1 == 2);
    std::cout << "✓ Basic test passed" << std::endl;
}

int main() {
    test_basic();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
]]
else
    testContent = [[#include <assert.h>
#include <stdio.h>

void test_basic(void) {
    assert(1 + 1 == 2);
    printf("✓ Basic test passed\n");
}

int main(void) {
    test_basic();
    printf("All tests passed!\n");
    return 0;
}
]]
end

local testFile = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/tests/" .. testFileName,
    testContent
)
print("   ✓ Created " .. testFileName .. " file object")

-- Create CMakeLists.txt
local cmakeContent
if language == "cpp" then
    cmakeContent = [[cmake_minimum_required(VERSION 3.15)
project(]] .. projectName .. [[)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(]] .. projectName .. [[ src/main.cpp)
add_executable(]] .. projectName .. [[_tests tests/test_main.cpp)
]]
else
    cmakeContent = [[cmake_minimum_required(VERSION 3.15)
project(]] .. projectName .. [[)

set(CMAKE_C_STANDARD 23)

include_directories(include)

add_executable(]] .. projectName .. [[ src/main.c)
add_executable(]] .. projectName .. [[_tests tests/test_main.c)
]]
end

local cmakeFile = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/CMakeLists.txt",
    cmakeContent
)
print("   ✓ Created CMakeLists.txt file object")

-- Create .gitignore
local gitignoreFile = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/.gitignore",
    [[# Build artifacts
build/
*.o
*.a
*.so
*.dylib

# IDE
.vscode/
.idea/
*.swp
*.swo

# OS
.DS_Store
Thumbs.db
]]
)
print("   ✓ Created .gitignore file object")

-- Build directory tree
print("\n4. Building Directory Tree:")
print("   ------------------------")

cdirnuts.append_file(rootDir, readmeFile)
print("   ✓ Added README.md to root directory")

cdirnuts.append_file(rootDir, cmakeFile)
print("   ✓ Added CMakeLists.txt to root directory")

cdirnuts.append_file(rootDir, gitignoreFile)
print("   ✓ Added .gitignore to root directory")

cdirnuts.append_file(srcDir, mainFile)
print("   ✓ Added " .. mainFileName .. " to src directory")

cdirnuts.append_file(includeDir, headerFile)
print("   ✓ Added app.h to include directory")

cdirnuts.append_file(testsDir, testFile)
print("   ✓ Added " .. testFileName .. " to tests directory")

cdirnuts.append_subdir(rootDir, srcDir)
print("   ✓ Added src to root directory")

cdirnuts.append_subdir(rootDir, includeDir)
print("   ✓ Added include to root directory")

cdirnuts.append_subdir(rootDir, testsDir)
print("   ✓ Added tests to root directory")

-- Write the entire structure to disk
print("\n5. Writing to Filesystem:")
print("   ----------------------")

cdirnuts.write_virtual_dir(rootDir)
print("   ✓ Successfully created entire project structure!")

cdirnuts.execute_shell_command("git init ")

print("\n=== Example Complete ===")
print("\nProject structure created in " .. cwd .. "/" .. projectName .. "/")
print("You can now:")
print("  1. cd " .. projectName)
print("  2. mkdir build && cd build")
print("  3. cmake ..")
print("  4. make")
print("  5. ./" .. projectName)
