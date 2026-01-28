#!/bin/bash
set -e

echo "Building C++ project with Conan..."

mkdir -p build
cd build

echo "Installing dependencies with Conan..."
conan install .. --build=missing -s compiler.libcxx=libstdc++11

echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

echo "Building project..."
cmake --build . --config Release --parallel $(nproc)

echo "Build completed successfully!"
echo "Executable: build/JsonFormatter"
