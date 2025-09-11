# Secure Password Generator

A modern, extensible C++ password generator library with CLI interface, built using professional design patterns and security best practices.

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

#### Option 1: Using Build Script (Recommended)

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

#### Option 2: Manual CMake Build

```bash
# Clone the repository
git clone <repository-url>
cd password-generator

# Build with CMake
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run tests
./run_tests.sh
```

### Installation

#### System-wide Installation

```bash
# Build and install system-wide (requires sudo)
./scripts/install.sh --system

# Now you can run from anywhere
passgen --help
```

#### User Installation (Default)

```bash
# Install for current user only
./scripts/install.sh --user

# Or simply
./scripts/install.sh

# Adds to ~/.local/bin and updates PATH
passgen --version
```

#### Uninstallation

```bash
# Remove installation
./scripts/install.sh --uninstall
```

### Basic Usage

#### Running from Build Directory

```bash
# Generate a single password (interactive mode)
./build/passgen

# Generate a single password (automated)
./build/passgen -g

# Show help
./build/passgen --help
```

#### After Installation

```bash
# Interactive mode (no arguments)
passgen

# Automated usage with flags
passgen -g                    # Generate one password
passgen -g -l 20              # Generate 20-character password
passgen -b 5                  # Generate 5 passwords
passgen -g --no-symbols       # Generate without symbols
passgen -p -l 12              # Generate pronounceable 12-char password
passgen -q -g                 # Generate quietly (for scripts)

# Show help and configuration
passgen --help
passgen -c                    # Show current config
passgen -v "mypassword123"    # Validate a password
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
passgen -g --no-lowercase --no-symbols

# Custom symbols only
passgen -g -s "!@#$" --no-lowercase --no-uppercase --no-digits

# Alphanumeric only (no symbols)
passgen -g --no-symbols -l 24
```

#### Batch Generation
```bash
# Generate 10 passwords
passgen -b 10

# Generate 5 pronounceable passwords quietly
passgen -b 5 -p -q

# Generate 3 long passwords without symbols
passgen -b 3 -l 32 --no-symbols
```

#### Validation and Configuration
```bash
# Check current settings
passgen -c

# Validate passwords
passgen -v "MyPassword123!"
passgen -v "weak" && echo "Valid" || echo "Invalid"

# Show help for all options
passgen --help
```

#### Scripting and Automation
```bash
# Generate password for scripts (quiet mode)
PASSWORD=$(passgen -g -q)

# Generate multiple passwords for a file
passgen -b 20 -q > passwords.txt

# Generate password with specific requirements
passgen -g -l 16 --no-symbols -q | tr -d '\n' | pbcopy  # macOS
passgen -g -l 16 --no-symbols -q | tr -d '\n' | xclip   # Linux

# Conditional generation
if passgen -v "$USER_PASSWORD" -q > /dev/null 2>&1; then
    echo "Password is valid"
else
    echo "Generating new password:"
    passgen -g -l 20
fi
```

### Output Modes

#### Normal Mode (default)
Provides formatted output with boxes, entropy information, and decorative elements:
```bash
passgen -g
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
passgen -g -q
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

- **`./scripts/build.sh`** - Comprehensive build script with dependency installation, testing, and coverage options
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
passgen -g

# Generate a 24-character password  
passgen -g -l 24

# Generate without symbols (alphanumeric only)
passgen -g --no-symbols

# Generate pronounceable password
passgen -p -l 14
```

#### Batch Generation for Teams
```bash
# Generate 10 passwords for team accounts
passgen -b 10 -l 20 --no-symbols

# Generate passwords quietly and save to file
passgen -b 50 -q > team_passwords.txt

# Generate pronounceable passwords for users
passgen -b 5 -p -l 12
```

#### Scripting and Integration
```bash
# Use in shell scripts
NEW_PASS=$(passgen -g -q -l 16)
echo "Generated password: $NEW_PASS"

# Validate existing passwords
if passgen -v "$CURRENT_PASSWORD" -q >/dev/null 2>&1; then
    echo "Password meets requirements"
else
    echo "Password needs updating"
    passgen -g -l 20
fi

# Custom character sets for specific systems
passgen -g -s "!@#$%^&*" --no-digits -l 12
```

#### Security Analysis
```bash
# Check password strength
passgen -v "MyCurrentPassword123!"

# Show current security settings
passgen -c

# Generate high-entropy passwords
passgen -g -l 32  # ~207 bits of entropy
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

- Linux (primary)
- macOS
- Windows (via MSYS2/MinGW)

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