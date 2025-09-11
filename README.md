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

### Basic Usage

```bash
# Generate a single password
./passgen

# Show help
./passgen --help

# Interactive mode
./passgen
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

The project includes comprehensive tests:

```bash
# Run all tests
./run_tests.sh

# Run specific test suites
./run_tests.sh --filter "*Validator*"

# Clean build and test
./run_tests.sh --clean

# Verbose output
./run_tests.sh --verbose
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