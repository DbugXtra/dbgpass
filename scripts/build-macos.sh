#!/bin/bash

# ============================================================================
# macOS-Specific Build Script for Secure Password Generator
# ============================================================================

set -e  # Exit on error

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Function to print colored output
print_color() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

# Function to print section headers
print_header() {
    echo
    print_color "$CYAN" "🍎 ┌──────────────────────────────────────────────────────────────┐"
    printf "${CYAN}🍎 │${NC} %-60s ${CYAN}│${NC}\n" "$1"
    print_color "$CYAN" "🍎 └──────────────────────────────────────────────────────────────┘"
}

# Function to check macOS version
check_macos_version() {
    local version=$(sw_vers -productVersion)
    local major=$(echo $version | cut -d. -f1)
    local minor=$(echo $version | cut -d. -f2)
    
    print_color "$BLUE" "macOS Version: $version"
    
    if [ "$major" -ge 11 ] || ([ "$major" -eq 10 ] && [ "$minor" -ge 15 ]); then
        print_color "$GREEN" "✓ macOS version supported"
    else
        print_color "$YELLOW" "⚠️  Old macOS version detected - may have compatibility issues"
    fi
}

# Function to detect architecture
detect_architecture() {
    local arch=$(uname -m)
    case $arch in
        arm64)
            print_color "$GREEN" "🚀 Apple Silicon (M1/M2/M3) detected"
            ARCH="Apple Silicon"
            ;;
        x86_64)
            print_color "$BLUE" "💻 Intel x86_64 detected"
            ARCH="Intel"
            ;;
        *)
            print_color "$YELLOW" "🤔 Unknown architecture: $arch"
            ARCH="Unknown"
            ;;
    esac
}

# Function to check and install Homebrew
setup_homebrew() {
    if ! command -v brew &> /dev/null; then
        print_color "$YELLOW" "🍺 Homebrew not found. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        
        # Add Homebrew to PATH for Apple Silicon
        if [[ $(uname -m) == "arm64" ]]; then
            echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
            eval "$(/opt/homebrew/bin/brew shellenv)"
        fi
    else
        print_color "$GREEN" "✓ Homebrew is installed"
        brew --version
    fi
}

# Function to install dependencies
install_dependencies() {
    print_header "Installing Dependencies via Homebrew"
    
    local packages=("cmake" "ninja")
    local missing_packages=()
    
    # Check which packages are missing
    for package in "${packages[@]}"; do
        if ! brew list $package &> /dev/null; then
            missing_packages+=($package)
        fi
    done
    
    if [ ${#missing_packages[@]} -eq 0 ]; then
        print_color "$GREEN" "✓ All required packages are already installed"
    else
        print_color "$YELLOW" "📦 Installing missing packages: ${missing_packages[*]}"
        brew install "${missing_packages[@]}"
    fi
    
    # Check for Xcode Command Line Tools
    if ! xcode-select -p &> /dev/null; then
        print_color "$YELLOW" "🔧 Installing Xcode Command Line Tools..."
        xcode-select --install
        print_color "$BLUE" "Please follow the installation prompts and re-run this script"
        exit 0
    else
        print_color "$GREEN" "✓ Xcode Command Line Tools are installed"
    fi
}

# Function to show usage
show_usage() {
    cat << EOF
🍎 macOS Build Script for Secure Password Generator

Usage: $0 [OPTIONS]

OPTIONS:
    -h, --help          Show this help message
    -q, --quick         Quick build (skip dependency checks)
    -t, --tests         Run tests after building
    -i, --install       Install after building (user installation)
    -c, --clipboard     Add clipboard integration examples
    
EXAMPLES:
    $0                  # Full build with dependency check
    $0 --quick --tests  # Quick build with tests
    $0 --install        # Build and install for current user

FEATURES:
    ✓ Automatic Homebrew setup
    ✓ Apple Silicon & Intel support
    ✓ Xcode Command Line Tools check
    ✓ Optimal build configuration for macOS

EOF
}

# Parse command line arguments
QUICK_BUILD=0
RUN_TESTS=0
INSTALL_AFTER=0
SHOW_CLIPBOARD=0

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -q|--quick)
            QUICK_BUILD=1
            shift
            ;;
        -t|--tests)
            RUN_TESTS=1
            shift
            ;;
        -i|--install)
            INSTALL_AFTER=1
            shift
            ;;
        -c|--clipboard)
            SHOW_CLIPBOARD=1
            shift
            ;;
        *)
            print_color "$RED" "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Main function
main() {
    print_header "macOS Build System for Secure Password Generator"
    
    # Change to project root
    cd "$PROJECT_ROOT"
    
    # System checks
    check_macos_version
    detect_architecture
    
    # Setup dependencies (unless quick build)
    if [ $QUICK_BUILD -eq 0 ]; then
        setup_homebrew
        install_dependencies
    fi
    
    # Build the project
    print_header "Building Project"
    
    local build_args="--tests"
    if [ $RUN_TESTS -eq 1 ]; then
        build_args="$build_args"
    fi
    
    print_color "$YELLOW" "🔨 Running build script..."
    ./scripts/build.sh $build_args
    
    # Install if requested
    if [ $INSTALL_AFTER -eq 1 ]; then
        print_header "Installing Password Generator"
        ./scripts/install.sh --user
    fi
    
    # Show clipboard integration examples
    if [ $SHOW_CLIPBOARD -eq 1 ] || [ $INSTALL_AFTER -eq 1 ]; then
        print_header "macOS Integration Examples"
        
        echo
        print_color "$CYAN" "📋 Clipboard Integration:"
        print_color "$WHITE" "  # Copy password to clipboard"
        print_color "$WHITE" "  passgen -g -q | tr -d '\\n' | pbcopy"
        echo
        print_color "$CYAN" "🔔 Notification Integration:"
        print_color "$WHITE" "  # Generate password with notification"
        print_color "$WHITE" "  passgen -g && osascript -e 'display notification \"New password generated\" with title \"Password Generator\"'"
        echo
        print_color "$CYAN" "🔐 Keychain Integration:"
        print_color "$WHITE" "  # Generate safe password for keychain"
        print_color "$WHITE" "  passgen -g -l 20 --no-symbols"
        echo
    fi
    
    # Success summary
    print_header "Build Complete! 🎉"
    
    print_color "$GREEN" "┌─────────────────────────────────────┐"
    print_color "$GREEN" "│  🍎 macOS Build Successful!        │"
    print_color "$GREEN" "├─────────────────────────────────────┤"
    print_color "$GREEN" "│ Architecture: $ARCH"
    print_color "$GREEN" "│ Executable:   build/passgen         │"
    
    if [ $INSTALL_AFTER -eq 1 ]; then
        print_color "$GREEN" "│ Installed:    ~/.local/bin/passgen  │"
    fi
    
    print_color "$GREEN" "└─────────────────────────────────────┘"
    
    echo
    if [ $INSTALL_AFTER -eq 1 ]; then
        print_color "$CYAN" "🚀 You can now run: passgen --help"
    else
        print_color "$CYAN" "🚀 To run: cd build && ./passgen"
        print_color "$CYAN" "📦 To install: ./scripts/install.sh --user"
    fi
    echo
}

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    print_color "$RED" "❌ This script is designed for macOS only"
    print_color "$YELLOW" "Please use ./scripts/build.sh for other platforms"
    exit 1
fi

# Run main function
main