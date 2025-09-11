# Password Generator Architecture

## Overview

The Secure Password Generator is built following SOLID principles and professional design patterns to ensure maintainability, extensibility, and testability. The architecture emphasizes separation of concerns, dependency injection, and plugin-based extensibility.

## Core Design Patterns

### 1. Strategy Pattern
Used for different password generation algorithms. The `IPasswordStrategy` interface allows for multiple implementations:

- `StandardPasswordStrategy`: Traditional random character selection from configurable character sets
- `PronounceablePasswordStrategy`: Creates memorable, pronounceable passwords using syllable patterns
- `PatternPasswordStrategy`: Generates passwords based on user-defined patterns (e.g., "LLDDSS" for letter-letter-digit-digit-symbol-symbol)

```cpp
// Strategy interface
class IPasswordStrategy {
public:
    virtual ~IPasswordStrategy() = default;
    virtual std::string generate(size_t length) = 0;
};
```

### 2. Factory Method Pattern
Character set providers and validators are created using factory methods, allowing for runtime configuration and easy extension:

```cpp
// Factory usage example
if (config.includeLowercase) {
    strategy->addCharacterSet(std::make_unique<LowercaseProvider>());
}
```

### 3. PIMPL Pattern
Used throughout to hide implementation details, provide ABI stability, and reduce compilation dependencies:

```cpp
class PasswordGenerator {
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
```

### 4. Observer Pattern
The validation system uses an observer-like pattern where multiple validators can be chained:

```cpp
class PasswordGenerator {
public:
    void addValidator(std::unique_ptr<IPasswordValidator> validator);
    bool validatePassword(const std::string& password) const;
};
```

### 5. Command Pattern
The CLI interface uses command pattern for handling user input:

```cpp
std::map<std::string, std::function<void()>> commands;
commands["generate"] = [this]() { generatePassword(); };
```

## Component Architecture

### Core Layer (`core/`)

**PasswordGenerator** - Main facade class that coordinates all components:
- Manages configuration
- Orchestrates strategy selection
- Handles validation pipeline
- Provides high-level API

**PasswordGeneratorConfig** - Configuration structure:
```cpp
struct PasswordGeneratorConfig {
    size_t length = 16;
    bool includeLowercase = true;
    bool includeUppercase = true;
    bool includeDigits = true;
    bool includeSymbols = true;
    std::string customSymbols = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    bool pronounceable = false;
    
    // Validation settings
    size_t minLength = 8;
    size_t maxLength = 128;
    bool requireMixedCase = true;
    bool requireDigits = true;
    bool requireSymbols = false;
};
```

### Interface Layer (`core/interfaces/`)

Defines contracts for all pluggable components:

- `IPasswordStrategy` - Password generation algorithms
- `IPasswordValidator` - Validation rules
- `ICharacterSetProvider` - Character set suppliers
- `IRandomGenerator` - Random number generation

### Strategy Layer (`strategies/`)

**StandardPasswordStrategy**:
- Uses configurable character set providers
- Ensures at least one character from each required set
- Applies Fisher-Yates shuffle for randomness

**PronounceablePasswordStrategy**:
- Uses consonant-vowel patterns
- Configurable capitalization and numbers
- Maintains readability while ensuring security

**PatternPasswordStrategy**:
- Pattern-based generation using format strings
- L=lowercase, U=uppercase, D=digit, S=symbol
- Supports literal characters in patterns

### Provider Layer (`providers/`)

Character set providers implement `ICharacterSetProvider`:
- `LowercaseProvider`: a-z
- `UppercaseProvider`: A-Z
- `DigitProvider`: 0-9
- `SymbolProvider`: Configurable symbol set

### Validator Layer (`validators/`)

Validation components implement `IPasswordValidator`:

**MinLengthValidator** / **MaxLengthValidator**:
- Simple length validation
- Configurable bounds

**CharacterTypeValidator**:
- Ensures required character types are present
- Configurable requirements for each type

**EntropyValidator**:
- Calculates Shannon entropy
- Validates minimum randomness threshold

### Utility Layer (`utils/`)

**SecureRandomGenerator**:
- Cryptographically secure random number generation
- Uses `std::random_device` and `std::mt19937`
- Thread-safe implementation

### CLI Layer (`cli/`)

**PasswordGeneratorCLI**:
- Interactive command-line interface
- Colorful output with box drawing
- Real-time configuration and generation

## Data Flow

```
User Input → CLI → PasswordGenerator → Strategy Selection
                ↓
         Configuration → Provider Setup → Character Sets
                ↓
         Password Generation → Validation Pipeline → Output
```

## Memory Management

- **RAII**: All resources managed automatically
- **Smart Pointers**: `unique_ptr` for ownership, `shared_ptr` where needed
- **Move Semantics**: Efficient resource transfer
- **No Raw Pointers**: Memory safety guaranteed

## Thread Safety

- **Strategy Objects**: Thread-safe for read operations
- **Random Generator**: Thread-local storage for generators
- **Immutable Config**: Configuration objects are immutable after creation
- **State Isolation**: No shared mutable state between threads

## Extension Points

### Adding New Strategies
1. Implement `IPasswordStrategy`
2. Add to factory/configuration system
3. Write unit tests

### Adding New Validators
1. Implement `IPasswordValidator`
2. Integrate with validation pipeline
3. Add configuration options

### Adding New Character Sets
1. Implement `ICharacterSetProvider`
2. Add to provider factory
3. Update configuration structure

## Error Handling

- **Exception Safety**: Strong exception safety guarantee
- **RAII**: Automatic cleanup on exceptions
- **Validation**: Input validation at API boundaries
- **Graceful Degradation**: Fallback mechanisms where appropriate

## Testing Architecture

### Unit Tests
- Mock objects for external dependencies
- Isolated component testing
- Property-based testing for algorithms

### Integration Tests
- End-to-end workflow validation
- Real-world usage scenarios
- Performance benchmarks

### Test Utilities
- `MockRandomGenerator`: Predictable random sequences
- Test fixtures for common setups
- Assertion helpers for complex validation

## Performance Considerations

- **Memory Pool**: Character set caching
- **Algorithm Efficiency**: O(n) generation algorithms
- **Random Number Reuse**: Efficient RNG seeding
- **String Optimization**: Reserve capacity, move semantics

## Security Architecture

- **Secure Random**: Cryptographically secure RNG
- **Memory Clearing**: Automatic cleanup of sensitive data
- **No Logging**: Passwords never logged or stored
- **Constant Time**: Timing attack resistance where possible

## Future Extensibility

The architecture supports future enhancements:

- **Plugin System**: Dynamic loading of strategies/validators
- **Network Policies**: Remote validation services
- **Persistent Config**: Configuration persistence
- **Metrics Collection**: Usage analytics (privacy-preserving)
- **GUI Interface**: Graphical user interface layer