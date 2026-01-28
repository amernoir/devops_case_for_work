SQLite Cross-Platform Build Automation

Project Overview:
A comprehensive build automation system for SQLite that demonstrates modern DevOps practices including cross-platform compilation, containerization, infrastructure as code, and continuous integration.

SQLite Shared Library Docker Build Guide
==========================================
This document provides step-by-step instructions for building SQLite 
as a shared library (.so) using Docker and CMake.

Table of Contents:
------------------
1. Prerequisites
2. Quick Start (5-minute build)
3. Detailed Step-by-Step Guide
4. Building Without Docker
5. Using the Library in Your Projects
6. Troubleshooting
7. Project Structure

1. PREREQUISITES
----------------

Required software:
- Docker Engine 20.10+ or Docker Desktop
- Git (for cloning the repository)
- 2GB free disk space
- Internet connection for downloading base images

Optional (for local builds):
- CMake 3.10+
- GCC 8+ or Clang 10+
- Make or Ninja

Verify Docker installation:
```bash
docker --version
docker run hello-world
QUICK START (5-MINUTE BUILD)

For those who want to build immediately:

Step 1: Extract the archive

bash
unzip sqlite_docker_project.zip
cd sqlite_docker_project
Step 2: Build the Docker image

bash
cd docker
docker build -t sqlite3-shared:latest .
Step 3: Verify the build

bash
docker run --rm sqlite3-shared:latest ls -la /usr/lib/libsqlite3.so
Step 4: Create a test container

bash
docker run -it --rm sqlite3-shared:latest /bin/bash
# Inside container:
sqlite3 --version  # If sqlite3 CLI is installed
ldd /usr/lib/libsqlite3.so  # Check dependencies
DETAILED STEP-BY-STEP GUIDE

Phase 1: Prepare the Build Environment
Step 1.1: Clone/Extract the project

bash
# If using git:
git clone <repository-url>
cd sqlite_docker_project

# If using zip archive:
unzip sqlite_docker_project.zip
cd sqlite_docker_project
Step 1.2: Examine the project structure

bash
tree -L 2
# Expected structure:
# ├── sources/          # SQLite source code
# ├── cmake/           # CMake configuration
# ├── docker/          # Docker build files
# ├── logs/            # Build logs (for reference)
# └── README.txt       # This file
Phase 2: Docker Build Process
Step 2.1: Navigate to docker directory

bash
cd docker
Step 2.2: Build the Docker image

bash
# Basic build command:
docker build -t sqlite3-shared:latest .

# With build arguments (optional):
docker build \
  --build-arg SQLITE_VERSION=3.43.2 \
  --build-arg BUILD_TYPE=Release \
  -t sqlite3-shared:latest .
Step 2.3: Monitor the build process
The build consists of two stages:

Builder stage: Compiles SQLite from source

Runtime stage: Creates minimal image with only the library

Watch for these success messages:

text
[1/2] Building C object...
[2/2] Linking C shared library...
Successfully built <image_id>
Successfully tagged sqlite3-shared:latest
Step 2.4: Check the built image

bash
# List Docker images
docker images | grep sqlite3-shared

# Get image details
docker inspect sqlite3-shared:latest

# Check image size (should be ~100-150MB)
docker images --format "table {{.Repository}}\t{{.Tag}}\t{{.Size}}"
Phase 3: Verification and Testing
Step 3.1: Run a test container

bash
# Quick test - check if library exists
docker run --rm sqlite3-shared:latest \
  sh -c "file /usr/lib/libsqlite3.so && ldd /usr/lib/libsqlite3.so"
Step 3.2: Test with a C program
Create a test C file on your host:

c
// test_sqlite.c
#include <stdio.h>
#include <sqlite3.h>

int main() {
    printf("SQLite version: %s\n", sqlite3_libversion());
    printf("SQLite source ID: %s\n", sqlite3_sourceid());
    return 0;
}
Step 3.3: Compile and run the test inside container

bash
# Copy test file to container and compile
cat > test_sqlite.c << 'EOF'
#include <stdio.h>
#include <sqlite3.h>
int main() {
    printf("SQLite version: %s\n", sqlite3_libversion());
    return 0;
}
EOF

# Compile inside the container
docker run --rm -v $(pwd):/app sqlite3-shared:latest \
  sh -c "cd /app && gcc test_sqlite.c -o test -lsqlite3 && ./test"
Step 3.4: Interactive testing

bash
# Start interactive container
docker run -it --rm sqlite3-shared:latest /bin/bash

# Inside container:
# 1. Check library location
ls -la /usr/lib/libsqlite3.so

# 2. Verify with ldd
ldd /usr/lib/libsqlite3.so

# 3. Simple compile test
cat > /tmp/test.c << 'END'
#include <sqlite3.h>
#include <stdio.h>
int main() { printf("%s\n", sqlite3_libversion()); return 0; }
END
gcc /tmp/test.c -o /tmp/test -lsqlite3
/tmp/test

# 4. Exit container
exit
BUILDING WITHOUT DOCKER

If Docker is not available, build locally:

Step 4.1: Install dependencies

bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake

# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake

# macOS with Homebrew
brew install cmake gcc
Step 4.2: Build SQLite

bash
cd sqlite_docker_project

# Create build directory
mkdir -p build && cd build

# Configure with CMake
cmake ../cmake -DCMAKE_BUILD_TYPE=Release

# Build the library
make -j$(nproc)

# Verify
ls -la libsqlite3.so
file libsqlite3.so
Step 4.3: Install locally (optional)

bash
sudo make install
sudo ldconfig

# Test installation
gcc -o test -x c - << 'EOF'
#include <sqlite3.h>
#include <stdio.h>
int main() { printf("%s\n", sqlite3_libversion()); return 0; }
EOF
./test
USING THE LIBRARY IN YOUR PROJECTS

Option A: Using the Docker image as base
dockerfile
# Your Dockerfile
FROM sqlite3-shared:latest

# Your application setup
COPY your_app.c .
RUN gcc your_app.c -o app -lsqlite3
CMD ["./app"]
Option B: Copy library from container
bash
# Extract library from Docker image
docker create --name sqlite_temp sqlite3-shared:latest
docker cp sqlite_temp:/usr/lib/libsqlite3.so ./libsqlite3.so
docker rm sqlite_temp

# Use in your project
gcc your_app.c -o your_app -L. -lsqlite3 -I./include
Option C: Multi-stage Docker build
dockerfile
# Multi-stage build example
FROM sqlite3-shared:latest as sqlite

FROM debian:12-slim
COPY --from=sqlite /usr/lib/libsqlite3.so /usr/lib/
RUN ldconfig

# Build your application
COPY src/ /app/src/
WORKDIR /app
RUN gcc src/main.c -o app -lsqlite3
CMD ["./app"]
TROUBLESHOOTING

Problem: "docker: command not found"
Solution: Install Docker first

bash
# Ubuntu/Debian
sudo apt update
sudo apt install docker.io
sudo systemctl start docker
sudo usermod -aG docker $USER
# Log out and back in

# macOS: Download Docker Desktop from docker.com
# Windows: Download Docker Desktop from docker.com
Problem: "No space left on device"
Solution: Clean Docker resources

bash
# Remove unused containers, images, volumes
docker system prune -a

# Remove all Docker data (warning: destructive)
docker system prune -a --volumes
Problem: "Permission denied" on /var/run/docker.sock
Solution: Add user to docker group

bash
sudo usermod -aG docker $USER
newgrp docker  # Or log out and back in
Problem: Build fails with compiler errors
Solution: Check the build logs

bash
# Build with detailed output
docker build --progress=plain -t sqlite3-shared .

# Check the logs directory
cat logs/docker_build.log
cat logs/sqlite_build.log
Problem: Library not found at runtime
Solution: Ensure proper linking

bash
# Set library path
export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH

# Or use rpath when compiling
gcc program.c -o program -lsqlite3 -Wl,-rpath,/usr/lib
This project showcases how to build SQLite as shared libraries (.dll/.so) across different platforms using CMake, with automated workflows for Linux and Windows environments.
