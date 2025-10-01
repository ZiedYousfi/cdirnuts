# cdirnuts Lua API Documentation

This document describes the Lua API bindings for the cdirnuts C library, which provides functionality for creating and managing directory structures programmatically.

## Table of Contents

1. [Overview](#overview)
2. [API Reference](#api-reference)
   - [Directory Functions](#directory-functions)
   - [File Functions](#file-functions)
   - [Path Functions](#path-functions)
   - [Command Functions](#command-functions)
3. [Examples](#examples)
4. [Memory Management](#memory-management)

## Overview

The cdirnuts Lua API allows you to:

- Create complex directory structures
- Generate files with content
- Parse and manipulate file paths
- Execute shell commands

All functions are available under the global `cdirnuts` table.

## API Reference

### Directory Functions

#### `cdirnuts.allocDir(path)`

Allocates a new directory object.

**Parameters:**

- `path` (string): The path where the directory should be created

**Returns:**

- Directory object on success
- `nil, error_message` on failure

**Example:**

```lua
local dir = cdirnuts.allocDir("./my_project")
if not dir then
    print("Failed to allocate directory")
end
```

#### `cdirnuts.createDir(dir)`

Creates the directory and all its subdirectories and files on the filesystem.

**Parameters:**

- `dir` (Directory): A directory object created with `allocDir`

**Returns:**

- `true` on success
- `false, error_message` on failure

**Example:**

```lua
local dir = cdirnuts.allocDir("./my_project")
if cdirnuts.createDir(dir) then
    print("Directory created successfully!")
end
```

#### `cdirnuts.addSubDirToDir(parentDir, childDir)`

Adds a subdirectory to a parent directory. **Important:** This transfers ownership of the child directory to the parent. After calling this function, you should not use or free the child directory object.

**Parameters:**

- `parentDir` (Directory): The parent directory object
- `childDir` (Directory): The child directory object (ownership transferred)

**Returns:**

- `true` on success
- `false, error_message` on failure

**Example:**

```lua
local parent = cdirnuts.allocDir("./project")
local child = cdirnuts.allocDir("./project/src")

if cdirnuts.addSubDirToDir(parent, child) then
    print("Subdirectory added")
    -- Note: 'child' should not be used after this point
end
```

### File Functions

#### `cdirnuts.createFile(path, content)`

Creates a file object (not written to disk yet).

**Parameters:**

- `path` (string): The file path
- `content` (string or nil): The file content (can be nil for empty file)

**Returns:**

- File object on success
- `nil, error_message` on failure

**Example:**

```lua
local file = cdirnuts.createFile(
    "./project/README.md",
    "# My Project\n\nProject description here."
)
```

#### `cdirnuts.writeFile(file)`

Writes a file object directly to the filesystem.

**Parameters:**

- `file` (File): A file object created with `createFile`

**Returns:**

- `true` on success
- `false, error_message` on failure

**Example:**

```lua
local file = cdirnuts.createFile("./test.txt", "Hello, World!")
if cdirnuts.writeFile(file) then
    print("File written successfully!")
end
```

#### `cdirnuts.addFileToDir(dir, file)`

Adds a file to a directory. The file will be created when `createDir` is called on the directory. This function makes a deep copy of the file, so you retain ownership of the original file object.

**Parameters:**

- `dir` (Directory): The directory object
- `file` (File): The file object (caller retains ownership)

**Returns:**

- `true` on success
- `false, error_message` on failure

**Example:**

```lua
local dir = cdirnuts.allocDir("./project")
local file = cdirnuts.createFile("./project/main.c", "#include <stdio.h>\n\nint main() { return 0; }")

if cdirnuts.addFileToDir(dir, file) then
    print("File added to directory")
    -- Note: 'file' can still be used after this
end
```

### Path Functions

#### `cdirnuts.parsePath(path)`

Parses a path string into its components.

**Parameters:**

- `path` (string): The path to parse

**Returns:**

- PathInfo object on success with methods:
  - `getName()`: Returns the name component
  - `getParentPath()`: Returns the parent path
  - `isDirectory()`: Returns true if path ends with `/`
- `nil, error_message` on failure

**Example:**

```lua
local info = cdirnuts.parsePath("/home/user/projects/myapp/")
if info then
    print("Name: " .. info:getName())
    print("Parent: " .. info:getParentPath())
    print("Is directory: " .. tostring(info:isDirectory()))
end
```

#### `cdirnuts.constructPath(dirName, parentDir)`

Constructs a full path by combining directory name and parent path.

**Parameters:**

- `dirName` (string): The directory or file name
- `parentDir` (string): The parent directory path

**Returns:**

- Full path string on success
- `nil, error_message` on failure

**Example:**

```lua
local fullPath = cdirnuts.constructPath("myfile.txt", "/home/user")
-- Returns: "/home/user/myfile.txt"
```

#### `cdirnuts.copySubstring(source, start, end)`

Copies a substring from a source string.

**Parameters:**

- `source` (string): The source string
- `start` (integer): Start index (0-based, inclusive)
- `end` (integer): End index (0-based, inclusive)

**Returns:**

- Substring on success
- `nil, error_message` on failure

**Example:**

```lua
local sub = cdirnuts.copySubstring("HelloWorld", 0, 4)
-- Returns: "Hello"
```

### Command Functions

#### `cdirnuts.executeCommand(command)`

Executes a shell command.

**Parameters:**

- `command` (string): The shell command to execute

**Returns:**

- `true` on success (exit code 0)
- `false, error_message` on failure

**Example:**

```lua
if cdirnuts.executeCommand("git init") then
    print("Git repository initialized")
end
```

## Examples

### Example 1: Simple Directory Structure

```lua
-- Create a basic project structure
local project = cdirnuts.allocDir("./hello_world")

local readme = cdirnuts.createFile(
    "./hello_world/README.md",
    "# Hello World\n\nA simple project."
)

cdirnuts.addFileToDir(project, readme)

if cdirnuts.createDir(project) then
    print("Project created!")
end
```

### Example 2: Nested Directories with Files

```lua
-- Create parent directory
local app = cdirnuts.allocDir("./myapp")

-- Create subdirectories
local src = cdirnuts.allocDir("./myapp/src")
local include = cdirnuts.allocDir("./myapp/include")

-- Create files
local mainFile = cdirnuts.createFile(
    "./myapp/src/main.c",
    "#include <stdio.h>\n\nint main() {\n    printf(\"Hello!\\n\");\n    return 0;\n}\n"
)

local headerFile = cdirnuts.createFile(
    "./myapp/include/app.h",
    "#pragma once\n\nvoid app_init(void);\n"
)

-- Add files to directories
cdirnuts.addFileToDir(src, mainFile)
cdirnuts.addFileToDir(include, headerFile)

-- Build directory tree
cdirnuts.addSubDirToDir(app, src)
cdirnuts.addSubDirToDir(app, include)

-- Create everything
cdirnuts.createDir(app)
```

### Example 3: Using Path Functions

```lua
-- Parse a path
local path = "/home/user/documents/report.pdf"
local info = cdirnuts.parsePath(path)

print("File name: " .. info:getName())
print("Directory: " .. info:getParentPath())

-- Construct new path
local newPath = cdirnuts.constructPath("backup.pdf", info:getParentPath())
print("New path: " .. newPath)
```

### Example 4: Running Commands

```lua
-- Create a git repository
local repo = cdirnuts.allocDir("./my_repo")

local gitignore = cdirnuts.createFile(
    "./my_repo/.gitignore",
    "*.o\n*.exe\nbuild/\n"
)

cdirnuts.addFileToDir(repo, gitignore)
cdirnuts.createDir(repo)

-- Initialize git
if cdirnuts.executeCommand("cd ./my_repo && git init") then
    print("Git repository initialized!")
end
```

## Memory Management

The cdirnuts Lua API uses Lua's garbage collector to automatically manage memory for all objects:

- **Directory objects**: Automatically freed when garbage collected
- **File objects**: Automatically freed when garbage collected
- **PathInfo objects**: Automatically freed when garbage collected

### Important Notes

1. **Transfer of Ownership**: When using `addSubDirToDir`, ownership of the child directory is transferred to the parent. The child should not be used after this call.

2. **Deep Copy**: When using `addFileToDir`, a deep copy of the file is made. You can continue to use the original file object.

3. **Automatic Cleanup**: All objects are automatically cleaned up by Lua's garbage collector, so you don't need to manually free them.

## Running Lua Scripts

To execute a Lua script with the cdirnuts API:

```bash
# Using the cdirnuts executable
./cdirnuts --lua example.lua

# Or with a standalone Lua interpreter (if you've built a module)
lua example.lua
```

## Error Handling

Most functions return two values on error:

- First value: `nil` or `false`
- Second value: Error message string

Always check return values:

```lua
local dir, err = cdirnuts.allocDir("./test")
if not dir then
    print("Error: " .. err)
    return
end
```

## Best Practices

1. **Check return values**: Always check if operations succeeded
2. **Use meaningful paths**: Use relative or absolute paths consistently
3. **Order of operations**: Create all files and subdirectories before calling `createDir`
4. **Error messages**: Use the error messages for debugging
5. **Path separators**: Use forward slashes `/` for cross-platform compatibility

## Complete Working Example

See `example.lua` and `simple_example.lua` in the project root for complete, runnable examples.
