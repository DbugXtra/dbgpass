#!/bin/bash

# Password Generator Test Runner
# This script builds and runs all tests for the password generator project

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help           Show this help message"
    echo "  -c, --clean          Clean build before running tests"
    echo "  -v, --verbose        Run tests with verbose output"
    echo "  -f, --filter FILTER  Run only tests matching the filter pattern"
    echo "  --build-only         Only build, don't run tests"
    echo "  --no-build           Skip build, only run tests"
    echo ""
    echo "Examples:"
    echo "  $0                                    # Build and run all tests"
    echo "  $0 --clean                          # Clean build and run all tests"
    echo "  $0 --filter 'PasswordGenerator*'    # Run only PasswordGenerator tests"
    echo "  $0 --verbose                        # Run tests with detailed output"
}

# Default options
CLEAN_BUILD=false
VERBOSE=false
FILTER=""
BUILD_ONLY=false
NO_BUILD=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        --build-only)
            BUILD_ONLY=true
            shift
            ;;
        --no-build)
            NO_BUILD=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_ROOT/build"

print_status "Password Generator Test Runner"
print_status "Project root: $PROJECT_ROOT"

# Change to project root
cd "$PROJECT_ROOT"

# Build phase
if [[ "$NO_BUILD" != true ]]; then
    print_status "Building project..."
    
    # Clean build if requested
    if [[ "$CLEAN_BUILD" == true ]]; then
        print_status "Cleaning previous build..."
        rm -rf "$BUILD_DIR"
    fi
    
    # Create build directory if it doesn't exist
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Configure with CMake
    print_status "Configuring with CMake..."
    if ! cmake ..; then
        print_error "CMake configuration failed!"
        exit 1
    fi
    
    # Build the project
    print_status "Compiling..."
    if ! make -j$(nproc); then
        print_error "Build failed!"
        exit 1
    fi
    
    print_success "Build completed successfully"
    
    # If build-only mode, exit here
    if [[ "$BUILD_ONLY" == true ]]; then
        print_success "Build-only mode: Exiting after successful build"
        exit 0
    fi
else
    print_status "Skipping build (--no-build specified)"
    cd "$BUILD_DIR"
fi

# Check if test executable exists
TEST_EXECUTABLE="$BUILD_DIR/tests/password_generator_tests"
if [[ ! -f "$TEST_EXECUTABLE" ]]; then
    print_error "Test executable not found at: $TEST_EXECUTABLE"
    print_error "Make sure the project has been built successfully"
    exit 1
fi

# Run tests
print_status "Running tests..."

# Prepare test command
TEST_CMD="$TEST_EXECUTABLE"

# Add filter if specified
if [[ -n "$FILTER" ]]; then
    TEST_CMD="$TEST_CMD --gtest_filter=\"$FILTER\""
    print_status "Running tests with filter: $FILTER"
fi

# Add verbose output if requested
if [[ "$VERBOSE" == true ]]; then
    TEST_CMD="$TEST_CMD --gtest_output=xml:test_results.xml"
    print_status "Verbose mode enabled"
fi

# Execute tests
print_status "Executing: $TEST_CMD"
echo "----------------------------------------"

if eval "$TEST_CMD"; then
    echo "----------------------------------------"
    print_success "All tests passed! ✅"
    
    # Show test summary if verbose
    if [[ "$VERBOSE" == true && -f "test_results.xml" ]]; then
        print_status "Test results saved to: $BUILD_DIR/test_results.xml"
    fi
    
    # Also test the main executable
    print_status "Testing main executable..."
    MAIN_EXECUTABLE="$BUILD_DIR/passgen"
    
    if [[ -f "$MAIN_EXECUTABLE" ]]; then
        print_status "Testing --version flag..."
        "$MAIN_EXECUTABLE" --version
        
        print_status "Testing --help flag..."
        "$MAIN_EXECUTABLE" --help
        
        print_success "Main executable works correctly ✅"
    else
        print_warning "Main executable not found at: $MAIN_EXECUTABLE"
    fi
    
    exit 0
else
    echo "----------------------------------------"
    print_error "Some tests failed! ❌"
    exit 1
fi