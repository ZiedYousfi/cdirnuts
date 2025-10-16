#!/usr/bin/env lua

-- Example Lua script demonstrating the cdirnuts API
-- This script creates a complete project structure with directories and files

print("=== cdirnuts Lua API Example ===\n")

-- The cdirnuts API is already registered globally when embedded
-- No need to require it

-- get the cwd
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

-- 1. Create a project structure
print("2. Creating Project Structure:")
print("   ---------------------------")

-- Create root directory
local rootDir = cdirnuts.allocDir(cwd .. "/" .. projectName)
print("   ✓ Allocated root directory: ./" .. projectName)

-- Create src directory
local srcDir = cdirnuts.allocDir(cwd .. "/" .. projectName .. "/src")
print("   ✓ Allocated src directory")

-- Create include directory
local includeDir = cdirnuts.allocDir(cwd .. "/" .. projectName .. "/include")
print("   ✓ Allocated include directory")

-- Create tests directory
local testsDir = cdirnuts.allocDir(cwd .. "/" .. projectName .. "/tests")
print("   ✓ Allocated tests directory")

-- 2. Create files
print("\n3. Creating Files:")
print("   ---------------")

-- Create README.md
local readmeFile = cdirnuts.createFile(
    cwd .. "/" .. projectName .. "/README.md",
    [[# Test Project

    This project was created using the cdirnuts Lua API!

    ## Features
    - Automatic project scaffolding
    - Lua-based configuration
    - Easy to extend
]]
)
if readmeFile then
    print("   ✓ Created README.md file object")
    local success = cdirnuts.addFileToDir(rootDir, readmeFile)
    if success then
        print("   ✓ Added README.md to root directory")
    end
end

-- Create main.c
local mainFile = cdirnuts.createFile(
    cwd .. "/" .. projectName .. "/src/main.c",
    [[#include <stdio.h>
#include "../include/app.h"

int main(int argc, char **argv) {
    printf("Hello from %s!\n", APP_NAME);
    return 0;
}
]]
)
if mainFile then
    print("   ✓ Created main.c file object")
    local success = cdirnuts.addFileToDir(srcDir, mainFile)
    if success then
        print("   ✓ Added main.c to src directory")
    end
end

-- Create header file
local headerFile = cdirnuts.createFile(
    cwd .. "/" .. projectName .. "/include/app.h",
    [[#pragma once

#include <stdio.h>

void app_init(void);
void app_run(void);
void app_cleanup(void);
]]
)
if headerFile then
    print("   ✓ Created app.h file object")
    local success = cdirnuts.addFileToDir(includeDir, headerFile)
    if success then
        print("   ✓ Added app.h to include directory")
    end
end

-- Create test file
local testFile = cdirnuts.createFile(
    cwd .. "/" .. projectName .. "/tests/test_main.c",
    [[#include <assert.h>
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
)
if testFile then
    print("   ✓ Created test_main.c file object")
    local success = cdirnuts.addFileToDir(testsDir, testFile)
    if success then
        print("   ✓ Added test_main.c to tests directory")
    end
end

-- Create CMakeLists.txt
local cmakeFile = cdirnuts.createFile(
    cwd .. "/" .. projectName .. "/CMakeLists.txt",
    [[cmake_minimum_required(VERSION 3.15)
project(]] .. projectName .. [[)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(]] .. projectName .. [[ src/main.c)
add_executable(]] .. projectName .. [[_tests tests/test_main.c)
]]
)
if cmakeFile then
    print("   ✓ Created CMakeLists.txt file object")
    local success = cdirnuts.addFileToDir(rootDir, cmakeFile)
    if success then
        print("   ✓ Added CMakeLists.txt to root directory")
    end
end

-- 3. Add subdirectories to root
print("\n4. Building Directory Tree:")
print("   ------------------------")

local success = cdirnuts.addSubDirToDir(rootDir, srcDir)
if success then
    print("   ✓ Added src to root directory")
end

success = cdirnuts.addSubDirToDir(rootDir, includeDir)
if success then
    print("   ✓ Added include to root directory")
end

success = cdirnuts.addSubDirToDir(rootDir, testsDir)
if success then
    print("   ✓ Added tests to root directory")
end

-- 5. Create the entire structure on disk
print("\n5. Writing to Filesystem:")
print("   ----------------------")

local success = cdirnuts.createDir(rootDir)
if success then
    print("   ✓ Successfully created entire project structure!")
else
    print("   ✗ Failed to create project structure")
    os.exit(1)
end

-- 5. Execute a command (optional)
print("\n6. Running Commands:")
print("   -----------------")

local success = cdirnuts.executeCommand("git init " .. cwd .. "/" .. projectName)
if success then
    print("   ✓ Command executed successfully")
else
    print("   ✗ Command execution failed")
end

print("\n=== Example Complete ===")
print("\nProject structure created in "..cwd.."/"..projectName.."/")
print("You can now:")
print("  1. cd "..projectName)
print("  2. mkdir build && cd build")
print("  3. cmake ..")
print("  4. make")
print("  5. ./"..projectName)
