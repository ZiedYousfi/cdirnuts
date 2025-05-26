# CDirNuts

A utility for quickly creating standardized C project directory structures.

## Overview

CDirNuts is a command-line tool that generates a complete C project structure with all the essential directories and files needed to start development immediately. It creates a professional project layout following C development best practices.

## Features

- Creates standardized C project directory structure
- Generates essential files (Makefile, README.md, .gitignore, LICENSE)
- Sets up source and include directories
- Supports custom project names and paths
- Includes a basic "Hello, World!" C program to get started
- Configuration file support (--config option)
- Fast and lightweight

## Installation

```bash
# Clone the repository
git clone https://github.com/ZiedYousfi/cdirnuts
cd cdirnuts

# Build the utility
make

# Run the utility
./cdirnuts
```

## Usage

```bash
# Create a new C project with default name "my_project"
./cdirnuts

# Create a new C project with custom name
./cdirnuts my-awesome-project

# Create a project with a specific path
./cdirnuts /path/to/my-project

# Show help
./cdirnuts --help

# Use a configuration file
./cdirnuts --config config.txt
```

## Generated Project Structure

```
my_project/
├── src/
│   └── main.c          # Basic "Hello, World!" program
├── include/            # Header files directory
├── Makefile           # Build configuration
├── README.md          # Project documentation
├── .gitignore         # Git ignore rules
└── LICENSE            # License file
```

### File Descriptions

- **src/main.c**: A basic C program with "Hello, World!" output
- **include/**: Directory for header files (.h)
- **Makefile**: Build script with gcc configuration (C17 standard)
- **README.md**: Project documentation template
- **.gitignore**: Ignores build artifacts (*.o, *.exe, *.out, build/, *.log)
- **LICENSE**: Empty license file for your project

## Command Line Options

- `--help`: Display help message and usage information
- `--config <file>`: Specify a configuration file path
- `[project_name]`: Optional project name (defaults to "my_project")

## Requirements

- GCC compiler
- Make utility
- Unix-like operating system (Linux, macOS, WSL)
- Standard C library

## Project Structure Details

The generated Makefile includes:

- GCC compiler with C17 standard
- Warning flags (-Wall -Wextra)
- Clean target for removing build artifacts
- Proper compilation rules for C projects

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source. See [LICENSE file](LICENCE.md) for details.
