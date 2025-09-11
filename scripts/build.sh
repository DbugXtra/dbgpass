#!/bin/bash

# ============================================================================
# dbgpass - Debug Industries Pass - Build Script
# ============================================================================

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="build"
BUILD_TYPE="Release"
CMAKE_GENERATOR=""
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
RUN_TESTS=0
VERBOSE=0
CLEAN_BUILD=0
INSTALL_DEPS=0
COVERAGE=0

# Function to print colored output
print_color() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

# Function to print section headers
print_header() {
    echo
    print_color "$CYAN" "┌──────────────────────────────────────────────────────────────┐"
    printf "${CYAN}│${NC} %-60s ${CYAN}│${NC}\n" "$1"
    print_color "$CYAN" "└──────────────────────────────────────────────────────────────┘"
}

# Function to check for required tools
check_requirements() {
    local missing_tools=()
    
    # Check for CMake
    if ! command -v cmake &> /dev/null; then
        missing_tools+=("cmake")
    fi
    
    # Check for C++ compiler
    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        missing_tools+=("g++ or clang++")
    fi
    
    # Check for Make or Ninja
    if ! command -v make &> /dev/null && ! command -v ninja &> /dev/null; then
        missing_tools+=("make or ninja")
    fi
    
    if [ ${#missing_tools[@]} -ne 0 ]; then
        print_color "$RED" "Error: Missing required tools:"
        for tool in "${missing_tools[@]}"; do
            print_color "$RED" "  - $tool"
        done
        echo
        print_color "$YELLOW" "Install missing tools and try again."
        exit 1
    fi
}

# Function to detect build system
detect_build_system() {
    if command -v ninja &> /dev/null; then
        CMAKE_GENERATOR="-G Ninja"
        print_color "$GREEN" "✓ Using Ninja build system"
    elif command -v make &> /dev/null; then
        CMAKE_GENERATOR=""  # Default to Make
        print_color "$GREEN" "✓ Using Make build system"
    fi
}

# Function to show usage
show_usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Build script for dbgpass - Debug Industries Pass

OPTIONS:
    -h, --help          Show this help message
    -d, --debug         Build in debug mode (default: Release)
    -c, --clean         Clean build directory before building
    -t, --tests         Run tests after building
    -v, --verbose       Enable verbose output
    -j, --jobs NUM      Number of parallel jobs (default: $JOBS)
    --coverage          Enable code coverage (requires gcov)
    --install-deps      Install dependencies (requires sudo)
    --ninja             Force use of Ninja build system
    --make              Force use of Make build system

EXAMPLES:
    $0                  # Standard release build
    $0 --debug --tests  # Debug build with tests
    $0 --clean -j 8     # Clean build with 8 parallel jobs
    $0 --coverage       # Build with code coverage

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=1
            shift
            ;;
        -t|--tests)
            RUN_TESTS=1
            shift
            ;;
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --coverage)
            COVERAGE=1
            BUILD_TYPE="Debug"  # Coverage requires debug symbols
            shift
            ;;
        --install-deps)
            INSTALL_DEPS=1
            shift
            ;;
        --ninja)
            CMAKE_GENERATOR="-G Ninja"
            shift
            ;;
        --make)
            CMAKE_GENERATOR=""
            shift
            ;;
        *)
            print_color "$RED" "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Main build process
main() {
    print_header "dbgpass - Debug Industries Pass Build System"
    
    # Change to project root
    cd "$PROJECT_ROOT"
    
    # Check requirements
    print_header "Checking Requirements"
    check_requirements
    detect_build_system
    
    # Install dependencies if requested
    if [ $INSTALL_DEPS -eq 1 ]; then
        print_header "Installing Dependencies"
        
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
            print_color "$YELLOW" "Installing Linux dependencies..."
            sudo apt-get update
            sudo apt-get install -y cmake g++ make ninja-build
            
            if [ $COVERAGE -eq 1 ]; then
                sudo apt-get install -y gcovr lcov
            fi
            
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            print_color "$YELLOW" "Installing macOS dependencies..."
            
            if ! command -v brew &> /dev/null; then
                print_color "$RED" "Homebrew not found. Please install Homebrew first."
                exit 1
            fi
            
            brew install cmake ninja
            
            if [ $COVERAGE -eq 1 ]; then
                brew install gcovr lcov
            fi
            
        else
            print_color "$RED" "Unsupported OS for automatic dependency installation"
            exit 1
        fi
        
        print_color "$GREEN" "✓ Dependencies installed"
    fi
    
    # Clean build directory if requested
    if [ $CLEAN_BUILD -eq 1 ]; then
        print_header "Cleaning Build Directory"
        if [ -d "$BUILD_DIR" ]; then
            print_color "$YELLOW" "Removing $BUILD_DIR..."
            rm -rf "$BUILD_DIR"
            print_color "$GREEN" "✓ Build directory cleaned"
        else
            print_color "$BLUE" "Build directory doesn't exist, skipping clean"
        fi
    fi
    
    # Create build directory
    if [ ! -d "$BUILD_DIR" ]; then
        print_color "$YELLOW" "Creating build directory..."
        mkdir -p "$BUILD_DIR"
    fi
    
    cd "$BUILD_DIR"
    
    # Configure
    print_header "Configuring Build"
    
    print_color "$BLUE" "Build Type: $BUILD_TYPE"
    print_color "$BLUE" "Parallel Jobs: $JOBS"
    
    CMAKE_ARGS=(
        "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
        "-DBUILD_TESTS=ON"
    )
    
    if [ $COVERAGE -eq 1 ]; then
        CMAKE_ARGS+=(
            "-DCMAKE_CXX_FLAGS=--coverage"
            "-DCMAKE_C_FLAGS=--coverage"
            "-DCMAKE_EXE_LINKER_FLAGS=--coverage"
        )
    fi
    
    if [ $VERBOSE -eq 1 ]; then
        CMAKE_ARGS+=("-DCMAKE_VERBOSE_MAKEFILE=ON")
    fi
    
    print_color "$YELLOW" "Running CMake..."
    cmake $CMAKE_GENERATOR "${CMAKE_ARGS[@]}" ..
    
    if [ $? -eq 0 ]; then
        print_color "$GREEN" "✓ Configuration successful"
    else
        print_color "$RED" "✗ Configuration failed"
        exit 1
    fi
    
    # Build
    print_header "Building Project"
    
    BUILD_CMD="cmake --build . --config $BUILD_TYPE -j $JOBS"
    
    if [ $VERBOSE -eq 1 ]; then
        BUILD_CMD="$BUILD_CMD --verbose"
    fi
    
    print_color "$YELLOW" "Building with $JOBS parallel jobs..."
    
    if $BUILD_CMD; then
        print_color "$GREEN" "✓ Build successful"
    else
        print_color "$RED" "✗ Build failed"
        exit 1
    fi
    
    # Run tests if requested
    if [ $RUN_TESTS -eq 1 ]; then
        print_header "Running Tests"
        
        if [ -f "password_generator_tests" ] || [ -f "password_generator_tests.exe" ]; then
            print_color "$YELLOW" "Executing test suite..."
            
            if [ $VERBOSE -eq 1 ]; then
                ctest --output-on-failure --verbose
            else
                ctest --output-on-failure
            fi
            
            if [ $? -eq 0 ]; then
                print_color "$GREEN" "✓ All tests passed"
            else
                print_color "$RED" "✗ Some tests failed"
                exit 1
            fi
            
            # Generate coverage report if enabled
            if [ $COVERAGE -eq 1 ]; then
                print_header "Generating Coverage Report"
                
                print_color "$YELLOW" "Generating coverage data..."
                
                # Create coverage directory
                mkdir -p coverage
                
                # Generate coverage report
                if command -v gcovr &> /dev/null; then
                    gcovr -r .. --html --html-details -o coverage/index.html
                    print_color "$GREEN" "✓ Coverage report generated: $BUILD_DIR/coverage/index.html"
                    
                    # Generate terminal report
                    gcovr -r .. --print-summary
                elif command -v lcov &> /dev/null; then
                    lcov --capture --directory . --output-file coverage.info
                    genhtml coverage.info --output-directory coverage
                    print_color "$GREEN" "✓ Coverage report generated: $BUILD_DIR/coverage/index.html"
                else
                    print_color "$YELLOW" "Coverage tools not found. Install gcovr or lcov."
                fi
            fi
        else
            print_color "$YELLOW" "Test executable not found, skipping tests"
        fi
    fi
    
    # Print summary
    print_header "Build Summary"
    
    print_color "$GREEN" "┌─────────────────────────────────────┐"
    print_color "$GREEN" "│     Build Completed Successfully    │"
    print_color "$GREEN" "├─────────────────────────────────────┤"
    print_color "$GREEN" "│ Type:       $BUILD_TYPE"
    print_color "$GREEN" "│ Directory:  $PROJECT_ROOT/$BUILD_DIR"
    print_color "$GREEN" "│ Executable: $BUILD_DIR/dbgpass"
    
    if [ $RUN_TESTS -eq 1 ]; then
        print_color "$GREEN" "│ Tests:      ✓ Passed"
    fi
    
    if [ $COVERAGE -eq 1 ] && [ -d "coverage" ]; then
        print_color "$GREEN" "│ Coverage:   $BUILD_DIR/coverage/"
    fi
    
    print_color "$GREEN" "└─────────────────────────────────────┘"
    
    echo
    print_color "$CYAN" "To run the password generator:"
    print_color "$WHITE" "  cd $BUILD_DIR && ./dbgpass"
    echo
}

# Run main function
main