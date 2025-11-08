# cdirnuts Lua API Documentation

This document describes the Lua API bindings for the cdirnuts C++ library, which provides functionality for creating and managing directory structures programmatically.

## Table of Contents

1. [Overview](#overview)
2. [API Reference](#api-reference)
   - [Directory Functions](#directory-functions)
   - [File Functions](#file-functions)
   - [Utility Functions](#utility-functions)
3. [Examples](#examples)
4. [Memory Management](#memory-management)

## Overview

The cdirnuts Lua API allows you to:

- Create complex directory structures in memory
- Generate files with content
- Build hierarchical file system structures
- Write entire directory trees to disk in one operation

All functions are available under the global `cdirnuts` table.

## API Reference

### Directory Functions

#### `cdirnuts.create_virtual_dir(path)`

Creates a virtual directory object in memory. This directory is not written to disk until you call `write_virtual_dir()`.

**Parameters:**

- `path` (string): The path where the directory should be created

**Returns:**

- Directory userdata object

**Example:**

```lua
local dir = cdirnuts.create_virtual_dir("./my_project")
```

**Note:** This function throws a Lua error if directory creation fails.

#### `cdirnuts.write_virtual_dir(dir)`

Writes the directory and all its subdirectories and files to the filesystem.

**Parameters:**

- `dir` (Directory): A directory object created with `create_virtual_dir`

**Returns:**

- Nothing (throws on error)

**Example:**

```lua
local dir = cdirnuts.create_virtual_dir("./my_project")
-- ... add files and subdirs ...
cdirnuts.write_virtual_dir(dir)
```

#### `cdirnuts.append_subdir(parentDir, childDir)`

Adds a subdirectory to a parent directory. **Important:** This transfers ownership of the child directory to the parent. After calling this function, the child directory object is moved and should not be used again.

**Parameters:**

- `parentDir` (Directory): The parent directory object
- `childDir` (Directory): The child directory object (ownership transferred)

**Returns:**

- Nothing

**Example:**

```lua
local parent = cdirnuts.create_virtual_dir("./project")
local child = cdirnuts.create_virtual_dir("./project/src")

cdirnuts.append_subdir(parent, child)
-- Note: 'child' has been moved and should not be used after this point
```

### File Functions

#### `cdirnuts.create_virtual_file(path, content)`

Creates a virtual file object in memory with the specified content.

**Parameters:**

- `path` (string): The file path
- `content` (string): The file content

**Returns:**

- File userdata object

**Example:**

```lua
local file = cdirnuts.create_virtual_file(
    "./project/README.md",
    "# My Project\n\nProject description here."
)
```

**Note:** This function throws a Lua error if file creation fails.

#### `cdirnuts.write_virtual_file(file)`

Writes a file object directly to the filesystem.

**Parameters:**

- `file` (File): A file object created with `create_virtual_file`

**Returns:**

- Nothing (throws on error)

**Example:**

```lua
local file = cdirnuts.create_virtual_file("./test.txt", "Hello, World!")
cdirnuts.write_virtual_file(file)
```

#### `cdirnuts.append_file(dir, file)`

Adds a file to a directory. The file will be created when `write_virtual_dir` is called on the directory. **Important:** This transfers ownership of the file to the directory.

**Parameters:**

- `dir` (Directory): The directory object
- `file` (File): The file object (ownership transferred)

**Returns:**

- Nothing

**Example:**

```lua
local dir = cdirnuts.create_virtual_dir("./project")
local file = cdirnuts.create_virtual_file("./project/main.c", "#include <stdio.h>\n\nint main() { return 0; }")

cdirnuts.append_file(dir, file)
-- Note: 'file' has been moved and should not be used after this
```

### Utility Functions

#### `cdirnuts.getCWD()`

Returns the current working directory.

**Parameters:**

- None

**Returns:**

- String containing the current working directory path

**Example:**

```lua
local cwd = cdirnuts.getCWD()
print("Working in: " .. cwd)
```

## Examples

### Example 1: Simple Directory Structure

```lua
-- Create a basic project structure
local project = cdirnuts.create_virtual_dir("./hello_world")

local readme = cdirnuts.create_virtual_file(
    "./hello_world/README.md",
    "# Hello World\n\nA simple project."
)

cdirnuts.append_file(project, readme)
cdirnuts.write_virtual_dir(project)
print("Project created!")
```

### Example 2: Nested Directories with Files

```lua
-- Get current directory
local cwd = cdirnuts.getCWD()

-- Create parent directory
local app = cdirnuts.create_virtual_dir(cwd .. "/myapp")

-- Create subdirectories
local src = cdirnuts.create_virtual_dir(cwd .. "/myapp/src")
local include = cdirnuts.create_virtual_dir(cwd .. "/myapp/include")

-- Create files
local mainFile = cdirnuts.create_virtual_file(
    cwd .. "/myapp/src/main.c",
    "#include <stdio.h>\n\nint main() {\n    printf(\"Hello!\\n\");\n    return 0;\n}\n"
)

local headerFile = cdirnuts.create_virtual_file(
    cwd .. "/myapp/include/app.h",
    "#pragma once\n\nvoid app_init(void);\n"
)

-- Add files to directories
cdirnuts.append_file(src, mainFile)
cdirnuts.append_file(include, headerFile)

-- Build directory tree
cdirnuts.append_subdir(app, src)
cdirnuts.append_subdir(app, include)

-- Create everything on disk
cdirnuts.write_virtual_dir(app)
```

### Example 3: Complete C++ Project

```lua
local cwd = cdirnuts.getCWD()
local projectName = "my_cpp_project"

-- Create root and subdirectories
local root = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName)
local src = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName .. "/src")
local include = cdirnuts.create_virtual_dir(cwd .. "/" .. projectName .. "/include")

-- Create source file
local mainCpp = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/src/main.cpp",
    [[#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
]]
)

-- Create CMakeLists.txt
local cmake = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/CMakeLists.txt",
    [[cmake_minimum_required(VERSION 3.15)
project(my_cpp_project)

set(CMAKE_CXX_STANDARD 23)

add_executable(my_cpp_project src/main.cpp)
]]
)

-- Create README
local readme = cdirnuts.create_virtual_file(
    cwd .. "/" .. projectName .. "/README.md",
    "# " .. projectName .. "\n\nA C++ project created with cdirnuts."
)

-- Assemble the structure
cdirnuts.append_file(src, mainCpp)
cdirnuts.append_file(root, cmake)
cdirnuts.append_file(root, readme)
cdirnuts.append_subdir(root, src)
cdirnuts.append_subdir(root, include)

-- Write to disk
cdirnuts.write_virtual_dir(root)
print("C++ project created successfully!")
```

### Example 4: Writing Individual Files

```lua
-- Create and write a single file directly
local configFile = cdirnuts.create_virtual_file(
    "./config.json",
    '{\n  "name": "myapp",\n  "version": "1.0.0"\n}\n'
)

cdirnuts.write_virtual_file(configFile)
print("Config file created!")
```

## Memory Management

The cdirnuts Lua API uses C++ RAII (Resource Acquisition Is Initialization) and Lua's userdata mechanism to automatically manage memory:

- **Directory objects**: Managed by sol2 userdata, automatically freed when garbage collected
- **File objects**: Managed by sol2 userdata, automatically freed when garbage collected
- **Move semantics**: Used internally for efficient ownership transfer

### Important Notes on Ownership

1. **Transfer of Ownership**:
   - `append_subdir(parent, child)`: Transfers ownership of `child` to `parent`. After this call, the `child` object has been moved and should **not** be used again.
   - `append_file(dir, file)`: Transfers ownership of `file` to `dir`. After this call, the `file` object has been moved and should **not** be used again.

2. **Automatic Cleanup**: All objects are automatically cleaned up by Lua's garbage collector working with sol2's userdata mechanism. You don't need to manually free them.

3. **Best Practice**: After transferring ownership via `append_subdir` or `append_file`, do not attempt to use the transferred object. It has been moved into the parent structure.

### Example of Correct Ownership Handling

```lua
local project = cdirnuts.create_virtual_dir("./myproject")
local src = cdirnuts.create_virtual_dir("./myproject/src")
local file = cdirnuts.create_virtual_file("./myproject/src/main.c", "int main() {}")

-- Transfer file to src directory
cdirnuts.append_file(src, file)
-- Do NOT use 'file' after this point!

-- Transfer src to project
cdirnuts.append_subdir(project, src)
-- Do NOT use 'src' after this point!

-- Only use 'project' from now on
cdirnuts.write_virtual_dir(project)
```

## Running Lua Scripts

To execute a Lua script with the cdirnuts API:

```bash
# Using the cdirnuts executable with default script
./build/cdirnuts

# Using a custom Lua script
./build/cdirnuts --config my_script.lua
```

## Error Handling

The new API uses Lua exceptions for error handling. Functions will throw Lua errors if they fail:

```lua
-- Error handling with pcall
local success, err = pcall(function()
    local dir = cdirnuts.create_virtual_dir("./test")
    cdirnuts.write_virtual_dir(dir)
end)

if not success then
    print("Error: " .. err)
end
```

## Best Practices

1. **Don't reuse transferred objects**: After calling `append_file` or `append_subdir`, the object has been moved and should not be used
2. **Use meaningful paths**: Use absolute paths or paths relative to `getCWD()` for clarity
3. **Build before writing**: Create all files and subdirectories before calling `write_virtual_dir`
4. **Handle errors**: Use `pcall` for error handling when necessary
5. **Path separators**: Use forward slashes `/` for cross-platform compatibility

## Complete Working Example

See `default_init.lua` in the project root for a complete, runnable example that demonstrates all features including:

- Interactive user input
- Directory and file creation
- Building nested structures
- Writing to the filesystem
