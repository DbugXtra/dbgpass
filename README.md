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
  - Batch password generation
  - Real-time entropy calculation
  - Password validation tools

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
# Generate a single password
./build/passgen

# Show help
./build/passgen --help
```

#### After Installation

```bash
# Generate a single password
passgen

# Show help
passgen --help

# Interactive mode
passgen
# Then use commands like:
# - generate
# - batch
# - config
# - toggle uppercase/lowercase/digits/symbols
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