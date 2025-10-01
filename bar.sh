#!/bin/bash

# Use out-of-source CMake build in ./build
set -e
cmake -S . -B build
JOBS=${JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)}
cmake --build build -- -j${JOBS}

# Run the executable
./build/cdirnuts
