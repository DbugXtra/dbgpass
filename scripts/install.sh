#!/bin/bash

# ============================================================================
# Secure Password Generator - Installation Script
# ============================================================================

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="build"
PREFIX="/usr/local"
BINARY_NAME="passgen"
SYSTEMWIDE=0
USER_INSTALL=0
UNINSTALL=0
BUILD_FIRST=1

# Function to print colored output
print_color() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

# Function to print section headers
print_header() {
    echo
    print_color "$CYAN" "╔══════════════════════════════════════════════════════════════╗"
    printf "${CYAN}║${NC} %-60s ${CYAN}║${NC}\n" "$1"
    print_color "$CYAN" "╚══════════════════════════════════════════════════════════════╝"
}

# Function to check if running as root
check_root() {
    if [ "$EUID" -eq 0 ]; then
        return 0
    else
        return 1
    fi
}

# Function to detect OS
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
        
        # Detect distribution
        if [ -f /etc/debian_version ]; then
            DISTRO="debian"
        elif [ -f /etc/redhat-release ]; then
            DISTRO="redhat"
        elif [ -f /etc/arch-release ]; then
            DISTRO="arch"
        else
            DISTRO="unknown"
        fi
        
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        DISTRO="macos"
    elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
        OS="windows"
        DISTRO="windows"
    else
        OS="unknown"
        DISTRO="unknown"
    fi
    
    print_color "$BLUE" "Detected OS: $OS ($DISTRO)"
}

# Function to show usage
show_usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Installation script for Secure Password Generator

OPTIONS:
    -h, --help          Show this help message
    -s, --system        Install system-wide (requires sudo)
    -u, --user          Install for current user only
    --prefix PATH       Installation prefix (default: $PREFIX)
    --uninstall         Uninstall the password generator
    --no-build          Skip building (assumes already built)

EXAMPLES:
    $0 --system         # System-wide installation
    $0 --user           # User-only installation
    $0 --uninstall      # Remove installation

DEFAULT:
    Installs to ~/.local/bin (user installation)

EOF
}

# Function to create desktop entry (Linux)
create_desktop_entry() {
    local desktop_file="password-generator.desktop"
    local desktop_dir
    
    if [ $SYSTEMWIDE -eq 1 ]; then
        desktop_dir="/usr/share/applications"
    else
        desktop_dir="$HOME/.local/share/applications"
        mkdir -p "$desktop_dir"
    fi
    
    cat > "/tmp/$desktop_file" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Password Generator
Comment=Secure Password Generator
Exec=$1
Icon=security-high
Terminal=true
Categories=Utility;Security;
Keywords=password;generator;security;
EOF
    
    if [ $SYSTEMWIDE -eq 1 ]; then
        sudo mv "/tmp/$desktop_file" "$desktop_dir/"
    else
        mv "/tmp/$desktop_file" "$desktop_dir/"
    fi
    
    print_color "$GREEN" "✓ Desktop entry created"
}

# Function to install man page
install_man_page() {
    local man_dir
    local man_file="$PROJECT_ROOT/docs/passgen.1"
    
    # Create basic man page if it doesn't exist
    if [ ! -f "$man_file" ]; then
        mkdir -p "$PROJECT_ROOT/docs"
        cat > "$man_file" << 'EOF'
.TH PASSGEN 1 "2024" "1.0.0" "Password Generator Manual"
.SH NAME
passgen \- secure password generator
.SH SYNOPSIS
.B passgen
[\fIOPTIONS\fR]
.SH DESCRIPTION
Secure Password Generator is a tool for creating strong, random passwords
with various configuration options.
.SH COMMANDS
.TP
.B generate
Generate a single password
.TP
.B batch
Generate multiple passwords
.TP
.B config
Show current configuration
.TP
.B help
Show help message
.TP
.B exit
Exit the program
.SH EXAMPLES
.TP
Generate a password:
.B passgen
.br
> generate
.TP
Generate 10 passwords:
.B passgen
.br
> batch
.br
> 10
.SH AUTHOR
Secure Password Generator Team
.SH COPYRIGHT
MIT License
EOF
    fi
    
    if [ $SYSTEMWIDE -eq 1 ]; then
        man_dir="$PREFIX/share/man/man1"
        sudo mkdir -p "$man_dir"
        sudo cp "$man_file" "$man_dir/"
        sudo gzip -f "$man_dir/passgen.1"
        sudo mandb 2>/dev/null || true
    else
        man_dir="$HOME/.local/share/man/man1"
        mkdir -p "$man_dir"
        cp "$man_file" "$man_dir/"
        gzip -f "$man_dir/passgen.1"
    fi
    
    print_color "$GREEN" "✓ Man page installed"
}

# Function to perform installation
perform_install() {
    local install_dir
    local binary_path="$PROJECT_ROOT/$BUILD_DIR/$BINARY_NAME"
    
    # Check if binary exists
    if [ ! -f "$binary_path" ] && [ ! -f "${binary_path}.exe" ]; then
        print_color "$RED" "Error: Binary not found at $binary_path"
        print_color "$YELLOW" "Please build the project first using ./scripts/build.sh"
        exit 1
    fi
    
    # Determine installation directory
    if [ $SYSTEMWIDE -eq 1 ]; then
        install_dir="$PREFIX/bin"
        
        print_color "$YELLOW" "Installing system-wide to $install_dir"
        print_color "$YELLOW" "This requires sudo privileges..."
        
        sudo mkdir -p "$install_dir"
        sudo cp "$binary_path" "$install_dir/"
        sudo chmod 755 "$install_dir/$BINARY_NAME"
        
        # Install headers and libraries for development
        if [ -d "$PROJECT_ROOT/include" ]; then
            sudo mkdir -p "$PREFIX/include/password_generator"
            sudo cp -r "$PROJECT_ROOT/include/"* "$PREFIX/include/password_generator/"
            print_color "$GREEN" "✓ Headers installed"
        fi
        
        # Install library if it exists
        if [ -f "$PROJECT_ROOT/$BUILD_DIR/libpassword_generator_lib.a" ]; then
            sudo mkdir -p "$PREFIX/lib"
            sudo cp "$PROJECT_ROOT/$BUILD_DIR/libpassword_generator_lib.a" "$PREFIX/lib/"
            print_color "$GREEN" "✓ Library installed"
        fi
        
    else
        # User installation
        install_dir="$HOME/.local/bin"
        
        print_color "$YELLOW" "Installing for current user to $install_dir"
        
        mkdir -p "$install_dir"
        cp "$binary_path" "$install_dir/"
        chmod 755 "$install_dir/$BINARY_NAME"
        
        # Add to PATH if not already there
        if [[ ":$PATH:" != *":$install_dir:"* ]]; then
            print_color "$YELLOW" "Adding $install_dir to PATH..."
            
            # Detect shell and update appropriate config file
            if [ -n "$BASH_VERSION" ]; then
                echo "export PATH=\"\$PATH:$install_dir\"" >> "$HOME/.bashrc"
                print_color "$GREEN" "✓ Added to ~/.bashrc"
            elif [ -n "$ZSH_VERSION" ]; then
                echo "export PATH=\"\$PATH:$install_dir\"" >> "$HOME/.zshrc"
                print_color "$GREEN" "✓ Added to ~/.zshrc"
            else
                print_color "$YELLOW" "Please add $install_dir to your PATH manually"
            fi
            
            print_color "$YELLOW" "Run 'source ~/.bashrc' or start a new terminal for PATH changes"
        fi
    fi
    
    print_color "$GREEN" "✓ Binary installed to $install_dir/$BINARY_NAME"
    
    # Create desktop entry on Linux
    if [ "$OS" == "linux" ]; then
        create_desktop_entry "$install_dir/$BINARY_NAME"
    fi
    
    # Install man page
    install_man_page
    
    # Create uninstall script
    create_uninstall_script "$install_dir"
}

# Function to create uninstall script
create_uninstall_script() {
    local install_dir="$1"
    local uninstall_script="$install_dir/uninstall-passgen.sh"
    
    if [ $SYSTEMWIDE -eq 1 ]; then
        cat > "/tmp/uninstall-passgen.sh" << EOF
#!/bin/bash
echo "Uninstalling Password Generator..."
sudo rm -f "$install_dir/$BINARY_NAME"
sudo rm -rf "$PREFIX/include/password_generator"
sudo rm -f "$PREFIX/lib/libpassword_generator_lib.a"
sudo rm -f "/usr/share/applications/password-generator.desktop"
sudo rm -f "$PREFIX/share/man/man1/passgen.1.gz"
echo "✓ Password Generator uninstalled"
EOF
        sudo mv "/tmp/uninstall-passgen.sh" "$uninstall_script"
        sudo chmod 755 "$uninstall_script"
    else
        cat > "$uninstall_script" << EOF
#!/bin/bash
echo "Uninstalling Password Generator..."
rm -f "$install_dir/$BINARY_NAME"
rm -f "$HOME/.local/share/applications/password-generator.desktop"
rm -f "$HOME/.local/share/man/man1/passgen.1.gz"
rm -f "$uninstall_script"
echo "✓ Password Generator uninstalled"
echo "Note: PATH modifications in shell config files were not removed"
EOF
        chmod 755 "$uninstall_script"
    fi
    
    print_color "$GREEN" "✓ Uninstall script created: $uninstall_script"
}

# Function to perform uninstallation
perform_uninstall() {
    print_header "Uninstalling Password Generator"
    
    local paths_to_remove=()
    
    # Check system-wide installation
    if [ -f "$PREFIX/bin/$BINARY_NAME" ]; then
        paths_to_remove+=("$PREFIX/bin/$BINARY_NAME")
        paths_to_remove+=("$PREFIX/include/password_generator")
        paths_to_remove+=("$PREFIX/lib/libpassword_generator_lib.a")
        paths_to_remove+=("/usr/share/applications/password-generator.desktop")
        paths_to_remove+=("$PREFIX/share/man/man1/passgen.1.gz")
        SYSTEMWIDE=1
    fi
    
    # Check user installation
    if [ -f "$HOME/.local/bin/$BINARY_NAME" ]; then
        paths_to_remove+=("$HOME/.local/bin/$BINARY_NAME")
        paths_to_remove+=("$HOME/.local/share/applications/password-generator.desktop")
        paths_to_remove+=("$HOME/.local/share/man/man1/passgen.1.gz")
        paths_to_remove+=("$HOME/.local/bin/uninstall-passgen.sh")
    fi
    
    if [ ${#paths_to_remove[@]} -eq 0 ]; then
        print_color "$YELLOW" "Password Generator is not installed"
        exit 0
    fi
    
    print_color "$YELLOW" "The following will be removed:"
    for path in "${paths_to_remove[@]}"; do
        if [ -e "$path" ]; then
            echo "  - $path"
        fi
    done
    
    echo
    read -p "Continue with uninstallation? (y/N) " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        for path in "${paths_to_remove[@]}"; do
            if [ -e "$path" ]; then
                if [ $SYSTEMWIDE -eq 1 ] && [[ "$path" == "$PREFIX"* || "$path" == "/usr"* ]]; then
                    sudo rm -rf "$path"
                else
                    rm -rf "$path"
                fi
            fi
        done
        
        print_color "$GREEN" "✓ Password Generator uninstalled successfully"
    else
        print_color "$YELLOW" "Uninstallation cancelled"
    fi
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -s|--system)
            SYSTEMWIDE=1
            shift
            ;;
        -u|--user)
            USER_INSTALL=1
            shift
            ;;
        --prefix)
            PREFIX="$2"
            shift 2
            ;;
        --uninstall)
            UNINSTALL=1
            shift
            ;;
        --no-build)
            BUILD_FIRST=0
            shift
            ;;
        *)
            print_color "$RED" "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Main installation process
main() {
    print_header "Secure Password Generator Installer"
    
    # Detect OS
    detect_os
    
    # Change to project root
    cd "$PROJECT_ROOT"
    
    # Handle uninstallation
    if [ $UNINSTALL -eq 1 ]; then
        perform_uninstall
        exit 0
    fi
    
    # Determine installation type
    if [ $SYSTEMWIDE -eq 0 ] && [ $USER_INSTALL -eq 0 ]; then
        # Default to user installation
        USER_INSTALL=1
    fi
    
    if [ $SYSTEMWIDE -eq 1 ] && [ $USER_INSTALL -eq 1 ]; then
        print_color "$RED" "Error: Cannot specify both --system and --user"
        exit 1
    fi
    
    # Build if needed
    if [ $BUILD_FIRST -eq 1 ]; then
        print_header "Building Project"
        
        if [ -f "$SCRIPT_DIR/build.sh" ]; then
            print_color "$YELLOW" "Running build script..."
            "$SCRIPT_DIR/build.sh"
        else
            print_color "$RED" "Build script not found"
            exit 1
        fi
    fi
    
    # Perform installation
    print_header "Installing Password Generator"
    perform_install
    
    # Print summary
    print_header "Installation Complete"
    
    print_color "$GREEN" "╔════════════════════════════════════════╗"
    print_color "$GREEN" "║   Installation Successful!             ║"
    print_color "$GREEN" "╠════════════════════════════════════════╣"
    
    if [ $SYSTEMWIDE -eq 1 ]; then
        print_color "$GREEN" "║ Type:     System-wide                  ║"
        print_color "$GREEN" "║ Location: $PREFIX/bin/$BINARY_NAME"
    else
        print_color "$GREEN" "║ Type:     User                         ║"
        print_color "$GREEN" "║ Location: ~/.local/bin/$BINARY_NAME"
    fi
    
    print_color "$GREEN" "╚════════════════════════════════════════╝"
    
    echo
    print_color "$CYAN" "You can now run the password generator with:"
    print_color "$WHITE" "  $BINARY_NAME"
    echo
    print_color "$CYAN" "For help, run:"
    print_color "$WHITE" "  $BINARY_NAME --help"
    print_color "$WHITE" "  man $BINARY_NAME"
    echo
}

# Run main function
main