# dbgpass

**Debug Industries Password Generator – Secure passwords, terminal‑first.**

A modern, extensible C++ dbgpass library with CLI interface, built using professional design patterns and security best practices.

Designed for developers and security professionals who need reliable password generation in terminal environments.

## Features

- **Multiple Password Strategies**
  - Standard random character selection
  - Pronounceable password generation
  - Pattern-based password generation (NEW)
  
- **Comprehensive Validation**
  - Length validation (min/max)
  - Character type requirements
  - Entropy validation (NEW)
  
- **Extensible Architecture**
  - Plugin-based character set providers
  - Strategy pattern for generation algorithms
  - Validator pattern for custom rules
  
- **Security Features**
  - Cryptographically secure random number generation
  - Memory-safe implementation with RAII
  - Thread-safe design
  
- **User-Friendly CLI**
  - Interactive mode with colorful output
  - Command-line flags for automated access (NEW)
  - Batch password generation
  - Real-time entropy calculation
  - Password validation tools
  - Quiet mode for scripting

## Quick Start

### Building the Project

#### macOS (Apple Silicon & Intel)

##### Prerequisites
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies (automatic via build script)
./scripts/build.sh --install-deps --tests
```

##### One-Command Build (Recommended)
```bash
# Clone and build with dependencies
git clone <repository-url>
cd password-generator

# Option 1: Use macOS-specific script (recommended for macOS)
./scripts/build-macos.sh --install

# Option 2: Use cross-platform script
./scripts/build.sh --install-deps --tests

# Option 3: If you already have CMake and Ninja/Make
./scripts/build.sh --tests
```

##### Manual Dependency Installation
```bash
# If you prefer to install dependencies manually
brew install cmake ninja

# Optional: for code coverage
brew install gcovr lcov

# Then build
./scripts/build.sh --tests
```

#### Linux (Ubuntu/Debian/Other)

##### Option 1: Using Build Script (Recommended)

```bash
# Clone the repository
git clone <repository-url>
cd password-generator

# Build with the provided script (includes tests)
./scripts/build.sh --tests

# Or for development with debug symbols
./scripts/build.sh --debug --tests --verbose

# Install dependencies automatically (Ubuntu/Debian)
./scripts/build.sh --install-deps --tests
```

#### Cross-Platform Manual Build

```bash
# Clone the repository
git clone <repository-url>
cd password-generator

# Build with CMake
mkdir build && cd build
cmake ..

# macOS: Use number of CPU cores
make -j$(sysctl -n hw.ncpu)

# Linux: Use number of CPU cores  
make -j$(nproc)

# Windows (MSYS2/MinGW): Use available cores
make -j$(nproc)

# Run tests
cd .. && ./run_tests.sh
```

### Installation

#### macOS Installation

```bash
# Option 1: User installation (recommended)
./scripts/install.sh --user
# Installs to ~/.local/bin and updates ~/.bashrc or ~/.zshrc

# Option 2: System-wide installation (requires admin password)
./scripts/install.sh --system
# Installs to /usr/local/bin (available to all users)

# Option 3: One-command build and install
./scripts/build.sh --install-deps --tests && ./scripts/install.sh --user

# Verify installation
dbgpass --help
```

#### Linux Installation

##### System-wide Installation

```bash
# Build and install system-wide (requires sudo)
./scripts/install.sh --system

# Now you can run from anywhere
dbgpass --help
```

##### User Installation (Default)

```bash
# Install for current user only
./scripts/install.sh --user

# Or simply
./scripts/install.sh

# Adds to ~/.local/bin and updates PATH
dbgpass --version
```

#### Uninstallation (All Platforms)

```bash
# Remove installation (works for both user and system installs)
./scripts/install.sh --uninstall
```

### Basic Usage

#### Running from Build Directory

```bash
# Generate a single password (interactive mode)
./build/dbgpass

# Generate a single password (automated)
./build/dbgpass -g

# Show help
./build/dbgpass --help
```

#### After Installation

```bash
# Interactive mode (no arguments)
dbgpass

# Automated usage with flags
dbgpass -g                    # Generate one password
dbgpass -g -l 20              # Generate 20-character password
dbgpass -b 5                  # Generate 5 passwords
dbgpass -g --no-symbols       # Generate without symbols
dbgpass -p -l 12              # Generate pronounceable 12-char password
dbgpass -q -g                 # Generate quietly (for scripts)

# Show help and configuration
dbgpass --help
dbgpass -c                    # Show current config
dbgpass -v "mypassword123"    # Validate a password
```

#### Interactive Mode Commands

When running without arguments, use these commands:
- `generate` - Generate a single password
- `batch` - Generate multiple passwords
- `config` - Show current configuration
- `set length` - Set password length
- `toggle lowercase/uppercase/digits/symbols/pronounceable` - Toggle options
- `set symbols` - Set custom symbol set
- `validate` - Validate a password
- `help` - Show available commands
- `exit` or `quit` - Exit the program

## Command-Line Reference

### Available Flags

#### Generation Options
- `-g, --generate` - Generate a single password
- `-b, --batch <count>` - Generate multiple passwords (1-100)
- `-l, --length <n>` - Set password length (8-128)
- `-p, --pronounceable` - Generate pronounceable passwords

#### Character Set Options
- `--no-lowercase` - Exclude lowercase characters (a-z)
- `--no-uppercase` - Exclude uppercase characters (A-Z)
- `--no-digits` - Exclude digit characters (0-9)
- `--no-symbols` - Exclude symbol characters
- `-s, --symbols <chars>` - Set custom symbol set

#### Utility Options
- `-c, --config` - Show current configuration
- `-v, --validate <password>` - Validate a password
- `-q, --quiet` - Suppress prompts and decorations (for scripting)
- `-h, --help` - Show help message
- `--version` - Show version information

### Advanced Examples

#### Custom Character Sets
```bash
# Only uppercase and digits
dbgpass -g --no-lowercase --no-symbols

# Custom symbols only
dbgpass -g -s "!@#$" --no-lowercase --no-uppercase --no-digits

# Alphanumeric only (no symbols)
dbgpass -g --no-symbols -l 24
```

#### Batch Generation
```bash
# Generate 10 passwords
dbgpass -b 10

# Generate 5 pronounceable passwords quietly
dbgpass -b 5 -p -q

# Generate 3 long passwords without symbols
dbgpass -b 3 -l 32 --no-symbols
```

#### Validation and Configuration
```bash
# Check current settings
dbgpass -c

# Validate passwords
dbgpass -v "MyPassword123!"
dbgpass -v "weak" && echo "Valid" || echo "Invalid"

# Show help for all options
dbgpass --help
```

#### Scripting and Automation
```bash
# Generate password for scripts (quiet mode)
PASSWORD=$(dbgpass -g -q)

# Generate multiple passwords for a file
dbgpass -b 20 -q > passwords.txt

# Generate password with specific requirements
dbgpass -g -l 16 --no-symbols -q | tr -d '\n' | pbcopy  # macOS
dbgpass -g -l 16 --no-symbols -q | tr -d '\n' | xclip   # Linux

# Conditional generation
if dbgpass -v "$USER_PASSWORD" -q > /dev/null 2>&1; then
    echo "Password is valid"
else
    echo "Generating new password:"
    dbgpass -g -l 20
fi
```

### Output Modes

#### Normal Mode (default)
Provides formatted output with boxes, entropy information, and decorative elements:
```bash
dbgpass -g
# ┌─ Generated Password ─────────────────┐
# │ kL8#mR2@pQ9$vN7&                     │
# ├──────────────────────────────────────┤
# │ Length: 16 characters                │
# │ Entropy: 103 bits                    │
# └──────────────────────────────────────┘
```

#### Quiet Mode (-q)
Provides minimal output suitable for scripting:
```bash
dbgpass -g -q
# kL8#mR2@pQ9$vN7&
```

### Library Usage

```cpp
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"

using namespace password_generator::core;

// Basic usage
config::PasswordGeneratorConfig config;
config.length = 16;
config.includeSymbols = true;

PasswordGenerator generator(config);
std::string password = generator.generate();

// Batch generation
auto passwords = generator.generateBatch(10);

// Validation
bool isValid = generator.validatePassword("mypassword123");
auto errors = generator.getValidationErrors("weak");
```

## Build Scripts

The project includes professional build and installation scripts:

- **`./scripts/build.sh`** - Cross-platform build script with dependency installation, testing, and coverage options
- **`./scripts/build-macos.sh`** - macOS-optimized build script with Homebrew integration and Apple Silicon support
- **`./scripts/install.sh`** - Installation script supporting system-wide and user installations with man pages and desktop entries  
- **`./run_tests.sh`** - Dedicated testing script with filtering and reporting options

All scripts include `--help` for detailed usage information.

## Architecture

The project follows SOLID principles and uses several design patterns:

- **Strategy Pattern**: Different password generation algorithms
- **Factory Pattern**: Character set and validator creation
- **PIMPL Pattern**: Implementation hiding and ABI stability
- **Observer Pattern**: Validation pipeline
- **Command Pattern**: CLI command handling

See [Architecture Documentation](docs/architecture.md) for detailed information.

## API Reference

Complete API documentation is available in [API Reference](docs/api-reference.md).

### Core Classes

- `PasswordGenerator`: Main facade class
- `PasswordGeneratorConfig`: Configuration structure
- `IPasswordStrategy`: Strategy interface for generation algorithms
- `IPasswordValidator`: Validator interface for custom rules
- `ICharacterSetProvider`: Character set provider interface

### Strategies

- `StandardPasswordStrategy`: Random character selection
- `PronounceablePasswordStrategy`: Memorable passwords
- `PatternPasswordStrategy`: Pattern-based generation (e.g., "LLLUDDD" → "abc12ef")

### Validators

- `MinLengthValidator` / `MaxLengthValidator`: Length validation
- `CharacterTypeValidator`: Character type requirements
- `EntropyValidator`: Entropy/randomness validation

## Examples

### CLI Usage Examples

#### Basic Password Generation
```bash
# Generate a single secure password
dbgpass -g

# Generate a 24-character password  
dbgpass -g -l 24

# Generate without symbols (alphanumeric only)
dbgpass -g --no-symbols

# Generate pronounceable password
dbgpass -p -l 14
```

#### Batch Generation for Teams
```bash
# Generate 10 passwords for team accounts
dbgpass -b 10 -l 20 --no-symbols

# Generate passwords quietly and save to file
dbgpass -b 50 -q > team_passwords.txt

# Generate pronounceable passwords for users
dbgpass -b 5 -p -l 12
```

#### Scripting and Integration
```bash
# Use in shell scripts
NEW_PASS=$(dbgpass -g -q -l 16)
echo "Generated password: $NEW_PASS"

# Validate existing passwords
if dbgpass -v "$CURRENT_PASSWORD" -q >/dev/null 2>&1; then
    echo "Password meets requirements"
else
    echo "Password needs updating"
    dbgpass -g -l 20
fi

# Custom character sets for specific systems
dbgpass -g -s "!@#$%^&*" --no-digits -l 12
```

#### Security Analysis
```bash
# Check password strength
dbgpass -v "MyCurrentPassword123!"

# Show current security settings
dbgpass -c

# Generate high-entropy passwords
dbgpass -g -l 32  # ~207 bits of entropy
```

### Library Usage Examples

### Custom Strategy Example

```cpp
#include "strategies/PatternPasswordStrategy.h"

// Generate passwords following a pattern
auto strategy = std::make_unique<PatternPasswordStrategy>("LLudss-LLDD");
// L=lowercase, U=uppercase, D=digit, S=symbol, other chars literal
std::string password = strategy->generate(10); // "ab3$2-CD45"
```

### Custom Validator Example

```cpp
#include "validators/EntropyValidator.h"

// Add entropy validation
auto generator = PasswordGenerator();
generator.addValidator(std::make_unique<EntropyValidator>(50.0)); // minimum 50 bits
```

### Batch Generation Example

```cpp
// Generate 100 passwords
auto passwords = generator.generateBatch(100);
for (const auto& pwd : passwords) {
    std::cout << pwd << " (entropy: " 
              << calculateEntropy(pwd) << " bits)\n";
}
```

## Testing

The project includes comprehensive tests with multiple ways to run them:

#### Using the Build Script with Tests

```bash
# Build and run tests in one command
./scripts/build.sh --tests

# Debug build with tests and coverage
./scripts/build.sh --debug --tests --coverage

# Clean build with tests
./scripts/build.sh --clean --tests --verbose
```

#### Using the Dedicated Test Script

```bash
# Run all tests (after building)
./run_tests.sh

# Run specific test suites
./run_tests.sh --filter "*Validator*"

# Clean build and test
./run_tests.sh --clean

# Verbose output
./run_tests.sh --verbose
```

#### Manual Testing

```bash
# From build directory
cd build
./tests/password_generator_tests

# With GoogleTest options
./tests/password_generator_tests --gtest_filter="*PasswordGenerator*"
```

### Test Coverage

- Unit tests for all components
- Integration tests for complete workflows
- Mock objects for isolated testing
- Property-based testing for validation rules

## Extending the Library

The library is designed for easy extension. See [Extending Guide](docs/extending-guide.md) for:

- Creating custom password strategies
- Implementing custom validators
- Adding new character set providers
- Writing plugins and extensions

## Security Considerations

- Uses `std::random_device` and `std::mt19937` for secure random generation
- Memory is automatically cleared (RAII)
- No password storage or logging
- Constant-time operations where possible
- Thread-safe by design

## Dependencies

- **C++17** or later
- **CMake** 3.10+
- **GoogleTest** (automatically downloaded)

## Platform Support

- **Linux (primary)** - Full support with all features
- **macOS (Apple Silicon & Intel)** - Full support with Homebrew integration
- **Windows** - Via MSYS2/MinGW

### macOS-Specific Features

- **Homebrew Integration**: Automatic dependency installation via `brew`
- **Clipboard Integration**: Direct password copying with `pbcopy`
- **Shell Detection**: Automatically updates `.bashrc` or `.zshrc` during user installation
- **Apple Silicon**: Native support for M1/M2/M3 processors
- **Xcode Integration**: Uses Xcode Command Line Tools for compilation

#### macOS Tips & Tricks

```bash
# Copy password directly to clipboard (no newline)
dbgpass -g -q | tr -d '\n' | pbcopy

# Generate password and use in Keychain Access
dbgpass -g -l 20 --no-symbols

# Quick password for scripts (quiet mode)
NEW_PASS=$(dbgpass -g -q)
echo "Generated: $NEW_PASS"

# Integration with macOS notifications
dbgpass -g && osascript -e 'display notification "New password generated" with title "dbgpass"'
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass: `./run_tests.sh`
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Changelog

### v1.0.0
- Initial release
- Core password generation functionality
- CLI interface
- Comprehensive test suite
- Documentation and examples
