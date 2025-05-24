# CDirNuts

A utility for quickly creating standardized C project directory structures.

## Overview

CDirNuts is a command-line tool that generates a complete C project structure with all the essential directories and files needed to start development immediately. It creates a professional project layout following C development best practices.

## Features

- Creates standardized C project directory structure
- Generates essential files (Makefile, README, etc.)
- Sets up source, header, and build directories
- Includes common configuration files
- Fast and lightweight

## Installation

```bash
# Clone the repository
git clone https://github.com/ZiedYousfi/cdirnuts
cd cdirnuts

# Build the utility
make

# Install (optional)
make install
```

## Usage

```bash
# Create a new C project structure
./cdirnuts <project-name>

# Example
./cdirnuts my-awesome-project
```

## Generated Project Structure

```
my-awesome-project/
├── src/
│   └── main.c
├── include/
│   └── project.h
├── tests/
│   └── test_main.c
├── docs/
├── Makefile
├── README.md
└── .gitignore
```

### Directory Descriptions

- **src/**: Source code files (.c)
- **include/**: Header files (.h)
- **lib/**: External libraries
- **tests/**: Unit tests
- **docs/**: Documentation

## Requirements

- GCC compiler
- Make utility
- Unix-like operating system (Linux, macOS, WSL)

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source. See [LICENSE file](LICENCE.md) for details.
