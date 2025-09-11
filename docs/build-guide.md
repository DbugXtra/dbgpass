# Build Guide

This guide provides detailed instructions for building, testing, and installing the Secure Password Generator on various platforms.

## Prerequisites

### Required Tools

- **C++17 Compiler**: GCC 7.0+, Clang 5.0+, or MSVC 2017+
- **CMake**: Version 3.10 or higher
- **Git**: For cloning the repository

### Optional Tools

- **Ninja**: Faster build system (recommended)
- **Clang-Format**: Code formatting
- **Valgrind**: Memory leak detection (Linux)
- **AddressSanitizer**: Runtime error detection

## Platform-Specific Setup

### Ubuntu/Debian

```bash
# Install build tools
sudo apt update
sudo apt install build-essential cmake git

# Optional tools
sudo apt install ninja-build clang-format valgrind

# For development
sudo apt install gdb lldb
```

### CentOS/RHEL/Fedora

```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake git

# Fedora
sudo dnf groupinstall "Development Tools"  
sudo dnf install cmake git ninja-build

# Optional tools
sudo dnf install clang valgrind gdb
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install required tools
brew install cmake git

# Optional tools
brew install ninja clang-format llvm
```

### Windows (MSYS2/MinGW)

```bash
# Install MSYS2 from https://www.msys2.org/

# In MSYS2 shell:
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-ninja
pacman -S git
```

### Windows (Visual Studio)

- Install Visual Studio 2017 or later with C++ workload
- Install CMake from https://cmake.org/download/
- Install Git for Windows

## Building the Project

### Quick Build

```bash
# Clone repository
git clone <repository-url>
cd password-generator

# Build with default settings
mkdir build && cd build
cmake ..
make -j$(nproc)  # Linux/macOS
# OR
cmake --build . --parallel  # Cross-platform
```

### Build with Ninja (Recommended)

```bash
mkdir build && cd build
cmake -G Ninja ..
ninja
```

### Visual Studio Build

```cmd
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

## Build Configuration Options

### CMake Options

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build  
cmake -DCMAKE_BUILD_TYPE=Release ..

# Release with debug info
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Disable tests
cmake -DBUILD_TESTS=OFF ..

# Custom install prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# Enable address sanitizer
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON ..
```

### Compiler-Specific Options

```bash
# Use Clang
cmake -DCMAKE_CXX_COMPILER=clang++ ..

# Use specific GCC version
cmake -DCMAKE_CXX_COMPILER=g++-9 ..

# Enable LTO (Link Time Optimization)
cmake -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON ..
```

### Advanced Build Configuration

Create a `build-config.cmake` file:

```cmake
# build-config.cmake
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type")
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard")
set(BUILD_TESTS ON CACHE BOOL "Build tests")
set(ENABLE_COVERAGE OFF CACHE BOOL "Enable coverage")
set(ENABLE_SANITIZERS OFF CACHE BOOL "Enable sanitizers")

# Compiler-specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined")
endif()
```

Use with:
```bash
cmake -C build-config.cmake ..
```

## Testing

### Running Tests

```bash
# From build directory

# Run all tests
./run_tests.sh

# Run with different options
./run_tests.sh --clean           # Clean build first
./run_tests.sh --verbose         # Verbose output
./run_tests.sh --filter "*Validator*"  # Run specific tests

# Manual test execution
./tests/password_generator_tests

# Run with GoogleTest options
./tests/password_generator_tests --gtest_filter="*PasswordGenerator*"
./tests/password_generator_tests --gtest_repeat=100
./tests/password_generator_tests --gtest_shuffle
```

### Memory Testing

```bash
# Valgrind (Linux)
valgrind --tool=memcheck --leak-check=full ./tests/password_generator_tests

# Address Sanitizer
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address" ..
make
./tests/password_generator_tests
```

### Coverage Testing

```bash
# Install gcov/lcov (Ubuntu)
sudo apt install lcov

# Build with coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" ..
make

# Run tests
./tests/password_generator_tests

# Generate coverage report
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_report

# View report
firefox coverage_report/index.html
```

## Installation

### System Installation

```bash
# From build directory
sudo make install

# Or with CMake
sudo cmake --install .

# Custom prefix
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
make install
```

### Packaging

#### Creating DEB Package

```bash
# Install packaging tools
sudo apt install checkinstall

# From build directory
sudo checkinstall make install
```

#### Creating RPM Package

```bash
# Install packaging tools
sudo dnf install rpm-build

# Create RPM spec file and build
# (Detailed RPM creation is beyond scope)
```

#### CPack

```bash
# From build directory
cpack

# Specific generator
cpack -G DEB
cpack -G ZIP
```

## Development Setup

### IDE Configuration

#### VS Code

Create `.vscode/settings.json`:

```json
{
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.generator": "Ninja",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "C_Cpp.default.cppStandard": "c++17",
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    }
}
```

Create `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build",
            "type": "shell",
            "command": "cmake",
            "args": ["--build", "build", "--parallel"],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        },
        {
            "label": "Test",
            "type": "shell",
            "command": "./run_tests.sh",
            "group": "test"
        }
    ]
}
```

#### CLion

- Open project root directory
- CLion will automatically detect CMakeLists.txt
- Configure build profiles in Settings → Build, Execution, Deployment → CMake

#### Visual Studio

- Use "Open Folder" to open project root
- Visual Studio will detect CMakeLists.txt automatically
- Configure build settings in CMakeSettings.json

### Code Formatting

Setup `.clang-format`:

```yaml
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 100
AllowShortFunctionsOnASingleLine: Empty
AllowShortIfStatementsOnASingleLine: false
AllowShortLoopsOnASingleLine: false
```

Format code:
```bash
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

### Pre-commit Hooks

Create `.pre-commit-config.yaml`:

```yaml
repos:
  - repo: local
    hooks:
      - id: clang-format
        name: clang-format
        entry: clang-format
        language: system
        files: \\.(cpp|h)$
        args: [-i]
      
      - id: cmake-format
        name: cmake-format
        entry: cmake-format
        language: system
        files: CMakeLists\\.txt$|.*\\.cmake$
        args: [-i]
        
      - id: tests
        name: run-tests
        entry: ./run_tests.sh
        language: system
        pass_filenames: false
        always_run: true
```

Install and setup:
```bash
pip install pre-commit
pre-commit install
```

## Troubleshooting

### Common Build Issues

#### CMake Version Too Old

```bash
# Error: CMake 3.10 or higher is required
# Solution: Install newer CMake

# Ubuntu/Debian
sudo apt install cmake-data cmake
# Or use snap
sudo snap install cmake --classic

# Build from source
wget https://cmake.org/files/v3.20/cmake-3.20.0.tar.gz
tar -xzf cmake-3.20.0.tar.gz
cd cmake-3.20.0
./bootstrap && make && sudo make install
```

#### Compiler Not Found

```bash
# Error: Could NOT find compiler
# Solution: Install build tools

# Ubuntu/Debian
sudo apt install build-essential

# CentOS/RHEL
sudo yum groupinstall "Development Tools"

# Set specific compiler
export CC=gcc-9
export CXX=g++-9
```

#### GoogleTest Download Fails

```bash
# Error: Failed to download GoogleTest
# Solution: Manual installation

# Install system GoogleTest
sudo apt install libgtest-dev libgmock-dev

# Or build offline
mkdir build && cd build
cmake -DBUILD_TESTS=OFF ..  # Disable tests temporarily
```

#### Link Errors

```bash
# Error: undefined reference to 'std::filesystem'
# Solution: Link with filesystem library (GCC < 9)

# Add to CMakeLists.txt:
target_link_libraries(target_name stdc++fs)

# Or use newer compiler
```

#### Windows-Specific Issues

```bash
# Error: 'M_PI' was not declared
# Solution: Define _USE_MATH_DEFINES

# Add to CMakeLists.txt:
if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
endif()

# Or use portable constants
const double PI = 3.14159265358979323846;
```

### Performance Issues

#### Slow Debug Builds

```bash
# Use RelWithDebInfo for debugging with optimizations
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Or optimize specific parts
#ifdef DEBUG
    // Debug-only code
#endif
```

#### Memory Usage

```bash
# Reduce parallel jobs if running out of memory
make -j2  # Instead of -j$(nproc)

# Monitor memory usage
htop  # Linux
Activity Monitor  # macOS
Task Manager  # Windows
```

### Runtime Issues

#### Segmentation Fault

```bash
# Debug with GDB
gdb ./passgen
(gdb) run
(gdb) bt  # Backtrace when crashed

# Or with AddressSanitizer
cmake -DCMAKE_CXX_FLAGS="-fsanitize=address" ..
```

#### Tests Failing

```bash
# Run specific test with verbose output
./tests/password_generator_tests --gtest_filter="TestName" --gtest_verbose

# Check for race conditions
./tests/password_generator_tests --gtest_repeat=100 --gtest_shuffle

# Run with debugger
gdb ./tests/password_generator_tests
```

### Clean Build

```bash
# Remove build directory
rm -rf build

# Clean CMake cache
rm -rf CMakeCache.txt CMakeFiles/

# Full clean and rebuild
git clean -fdx  # WARNING: Removes all untracked files
mkdir build && cd build
cmake ..
make
```

## Continuous Integration

### GitHub Actions Example

`.github/workflows/ci.yml`:

```yaml
name: CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        compiler: [gcc, clang]
        exclude:
          - os: windows-latest
            compiler: clang

    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies (Ubuntu)
      if: matrix.os == 'ubuntu-latest'
      run: |
        sudo apt update
        sudo apt install ninja-build
    
    - name: Install dependencies (macOS)
      if: matrix.os == 'macos-latest'
      run: |
        brew install ninja
    
    - name: Configure
      run: |
        mkdir build
        cd build
        cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
    
    - name: Build
      run: |
        cd build
        ninja
    
    - name: Test
      run: |
        ./run_tests.sh --no-build
```

This comprehensive build guide should help users successfully build, test, and deploy the Secure Password Generator on any supported platform.