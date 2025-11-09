# cdirnuts Tests

This directory contains the test suite for the cdirnuts project.

## Running Tests

After building the project with CMake, you can run the tests using:

```bash
cd build
ctest --output-on-failure
```

Or run the test executable directly:

```bash
./build/cdirnuts_tests
```

## Test Organization

- `test_fs.cpp` - Tests for the file system module (`fs.h`/`fs.cpp`)
  - Path construction and manipulation
  - File creation and writing
  - Directory creation and nested structures
  - Complex directory hierarchies

- `test_presets.cpp` - Tests for the presets module (`presets.h`/`presets.cpp`)
  - Preset creation and management
  - PresetManager operations (add, remove, list, get)
  - Preset persistence (save/load from file)
  - Edge cases and error handling

- `test_lua.cpp` - Tests for the Lua engine module (`lua.h`/`lua.cpp`)
  - LuaEngine initialization
  - Script execution (string and file)
  - Lua API bindings (cdirnuts table)
  - Integration with file system operations
  - Error handling

## Test Coverage

The test suite covers:

- ✅ Unit tests for all major components
- ✅ Integration tests for Lua API
- ✅ File I/O operations
- ✅ Error handling and edge cases
- ✅ Complex nested structures

## Dependencies

Tests use Google Test (GTest) framework, which is automatically managed by vcpkg.
