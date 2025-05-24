#!/bin/bash

# Build the project
if make; then
  # Run the executable if build was successful
  ./build/hashmap
else
  echo "Build failed!"
  exit 1
fi
