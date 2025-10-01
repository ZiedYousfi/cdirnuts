#!/usr/bin/env lua

-- Example Lua script demonstrating the cdirnuts API
-- This script creates a complete project structure with directories and files

print("=== cdirnuts Lua API Example ===\n")

-- 1. Path utilities demonstration
print("1. Path Utilities:")
print("   ----------------")

local testPath = "/home/user/projects/myapp/"
local pathInfo = cdirnuts.parsePath(testPath)

if pathInfo then
    print("   Parsing path: " .. testPath)
    print("   - Name: " .. (pathInfo:getName() or "nil"))
    print("   - Parent path: " .. (pathInfo:getParentPath() or "nil"))
    print("   - Is directory: " .. tostring(pathInfo:isDirectory()))
    print()
end

local constructedPath = cdirnuts.constructPath("subdir", "/home/user")
if constructedPath then
    print("   Constructed path: " .. constructedPath)
    print()
end

local substring = cdirnuts.copySubstring("HelloWorld", 0, 4)
if substring then
    print("   Substring of 'HelloWorld' [0-4]: " .. substring)
    print()
end

-- 2. Create a project structure
print("2. Creating Project Structure:")
print("   ---------------------------")

-- Create root directory
local rootDir = cdirnuts.allocDir("./test_project")
print("   ✓ Allocated root directory: ./test_project")

-- Create src directory
local srcDir = cdirnuts.allocDir("./test_project/src")
print("   ✓ Allocated src directory")

-- Create include directory
local includeDir = cdirnuts.allocDir("./test_project/include")
print("   ✓ Allocated include directory")

-- Create tests directory
local testsDir = cdirnuts.allocDir("./test_project/tests")
print("   ✓ Allocated tests directory")

-- 3. Create files
print("\n3. Creating Files:")
print("   ---------------")

-- Create README.md
local readmeFile = cdirnuts.createFile(
    "./test_project/README.md",
    "# Test Project\n\nThis project was created using the cdirnuts Lua API!\n\n## Features\n- Automatic project scaffolding\n- Lua-based configuration\n- Easy to extend\n"
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
    "./test_project/src/main.c",
    [[#include <stdio.h>
#include "../include/app.h"

int main(int argc, char *argv[]) {
    printf("Hello from test_project!\n");
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
    "./test_project/include/app.h",
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
    "./test_project/tests/test_main.c",
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
    "./test_project/CMakeLists.txt",
    [[cmake_minimum_required(VERSION 3.15)
project(test_project)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(test_project src/main.c)
add_executable(test_main tests/test_main.c)
]]
)
if cmakeFile then
    print("   ✓ Created CMakeLists.txt file object")
    local success = cdirnuts.addFileToDir(rootDir, cmakeFile)
    if success then
        print("   ✓ Added CMakeLists.txt to root directory")
    end
end

-- 4. Add subdirectories to root
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

-- 6. Execute a command (optional)
print("\n6. Running Commands:")
print("   -----------------")

local success = cdirnuts.executeCommand("ls -la ./test_project")
if success then
    print("   ✓ Command executed successfully")
else
    print("   ✗ Command execution failed")
end

print("\n=== Example Complete ===")
print("\nProject structure created in ./test_project/")
print("You can now:")
print("  1. cd test_project")
print("  2. mkdir build && cd build")
print("  3. cmake ..")
print("  4. make")
print("  5. ./test_project")
