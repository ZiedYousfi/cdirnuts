#!/bin/bash

# Use out-of-source CMake build in ./build
set -e
mkdir -p build
cmake -S . -B build
cmake --build build -- -j

# Run the executable
./build/cdirnuts
