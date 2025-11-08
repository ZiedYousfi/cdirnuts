# CDirNuts

A powerful and flexible utility for creating and managing C project directory structures with Lua scripting support.

## Overview

CDirNuts is a command-line tool that enables you to:

- Generate complete C/C++ project structures with customizable templates
- Use **Lua scripting** for advanced project generation and automation
- Save and reuse project templates as **presets**
- Execute shell commands and create complex directory hierarchies programmatically

Whether you need a quick default C/C++ project or a sophisticated custom structure, CDirNuts provides the flexibility to match your workflow.

## Features

### Core Functionality

- **Default Project Generation**: Creates standardized C/C++ project directory structure
- **Lua Scripting API**: Full programmatic control over project generation
- **Preset System**: Save and reuse custom project templates
- **Path Manipulation**: Advanced path parsing and construction utilities
- **Command Execution**: Run shell commands from within Lua scripts
- **File & Directory Management**: Create complex nested structures with content

### What's Included

- Generates essential files (Makefile, README.md, .gitignore, LICENSE)
- Sets up source and include directories
- Includes a basic "Hello, World!" C/C++ program for default projects
- Lua-based configuration for unlimited customization

## Installation

```bash
# Clone the repository
git clone https://github.com/ZiedYousfi/cdirnuts
cd cdirnuts

# Configure and build using CMake with vcpkg
cmake --preset=default
cmake --build build

# Run the utility
./build/cdirnuts
```

### Requirements

- CMake 3.13 or higher
- C23 compatible compiler (GCC, Clang)
- Lua 5.4 (automatically managed via vcpkg)
- vcpkg (for dependency management)

## Usage

### Basic Usage

```bash
# Create a new C project using the default_init.lua script
./build/cdirnuts

# Show help
./build/cdirnuts --help
```

### Lua Scripting

Execute Lua scripts to create custom project structures:

```bash
./build/cdirnuts --config my_template.lua
```

### Preset Management

Save and reuse project templates:

```bash
# List all saved presets
./build/cdirnuts --preset list

# Add a new preset
./build/cdirnuts --preset add my_template /path/to/template.lua

# Use a preset by name
./build/cdirnuts --preset my_template

# Remove a preset
./build/cdirnuts --preset remove my_template
```

Presets are stored in the path specified by the `CDIRNUTS_DIR_PATH` environment variable or default to `./`.

## Lua API

CDirNuts provides a comprehensive Lua API for creating custom project structures. See [LUA_API.md](LUA_API.md) for complete documentation.

### Quick Example

```lua
-- Create a simple project with custom structure

-- Get current working directory
local cwd = cdirnuts.getCWD()

-- Allocate directories
local project = cdirnuts.create_virtual_dir(cwd .. "/my_app")
local src = cdirnuts.create_virtual_dir(cwd .. "/my_app/src")

-- Create files
local mainFile = cdirnuts.create_virtual_file(
    cwd .. "/my_app/src/main.c",
    "#include <stdio.h>\n\nint main() {\n    printf(\"Hello!\\n\");\n    return 0;\n}\n"
)

-- Build the structure
cdirnuts.append_file(src, mainFile)
cdirnuts.append_subdir(project, src)

-- Create on filesystem
cdirnuts.write_virtual_dir(project)
```

### Available Lua Functions

- **Directory Management**: `create_virtual_dir()`, `write_virtual_dir()`, `append_subdir()`
- **File Operations**: `create_virtual_file()`, `write_virtual_file()`, `append_file()`
- **Utilities**: `getCWD()`, `execute_shell_command()`

See `default_init.lua` in the repository for a complete working example.

## Command Line Options

- `--help`: Display help message and usage information
- `--config <file>`: Alias for `--lua` (legacy support)
- `--preset list`: List all saved presets
- `--preset add <name> <path>`: Add a new preset
- `--preset remove <name>`: Remove a preset
- `--preset <name>`: Use a saved preset

## Examples

### Example 1: Basic Default Project

```bash
./build/cdirnuts
```

Creates a standard C project structure using the built-in `default_init.lua` script.

### Example 2: Using Lua Script

```bash
./build/cdirnuts --config default_init.lua
```

Runs the provided `default_init.lua` script to create a custom project with:

- Interactive project name prompt
- Multiple directories (src, include, tests)
- CMakeLists.txt configuration
- Custom file templates

### Example 3: Creating and Using Presets

```bash
# Create a custom Lua template
cat > my_template.lua << 'EOF'
local cwd = cdirnuts.getCWD()
local project = cdirnuts.create_virtual_dir(cwd .. "/microservice")
-- ... add your custom structure
cdirnuts.write_virtual_dir(project)
EOF

# Save as preset
./build/cdirnuts --preset add microservice my_template.lua

# Use it anytime
./build/cdirnuts --preset microservice
```

## Project Architecture

CDirNuts is built with a modular architecture:

- **Core Modules** (`src/`):
  - `main.c` - Entry point and CLI argument parsing
  - `dir.c` - Directory creation and management
  - `path.c` - Path parsing and manipulation
  - `config.c` - Command execution utilities
  - `lua_embed.c` - Lua API bindings
  - `preset.c` - Preset storage and retrieval
  - `log.c` - Logging functionality
  - `init.c` - Default project initialization

- **Headers** (`include/`): Public API declarations

- **Build System**: CMake with vcpkg for dependency management

## Testing

CDirNuts includes a comprehensive test suite using Google Test (GTest).

```bash
# Build the project with tests (testing is enabled by default)
cmake --preset vcpkg
cmake --build build

# Run tests using CTest
cd build
ctest --output-on-failure

# Or run the test executable directly
./build/cdirnuts_tests
```

### Test Coverage

The test suite includes:

- **File System Tests** (`test_fs.cpp`): Path manipulation, file/directory creation, nested structures
- **Preset Tests** (`test_presets.cpp`): Preset management, save/load operations, edge cases
- **Lua Engine Tests** (`test_lua.cpp`): Lua API bindings, script execution, error handling

All 48 tests verify core functionality and ensure reliability across different scenarios.

See [tests/README.md](tests/README.md) for more information about the test suite.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feat/amazing-feature`)
3. Make your changes
4. Test thoroughly (build and run tests)
5. Commit your changes (`git commit -m 'Add amazing feature'`)
6. Push to the branch (`git push origin feat/amazing-feature`)
7. Submit a pull request

## License

This project is open source. See [LICENSE file](LICENCE.md) for details.

## Documentation

- [Lua API Documentation](LUA_API.md) - Complete reference for the Lua scripting API
- [Default Script](default_init.lua) - Fully commented example demonstrating all features

## Support

For issues, questions, or contributions, please visit the [GitHub repository](https://github.com/ZiedYousfi/cdirnuts).
