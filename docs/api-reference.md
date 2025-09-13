# API Reference

## Core Classes

### PasswordGenerator

Main facade class that provides the primary interface for password generation.

```cpp
#include "core/PasswordGenerator.h"

namespace password_generator::core {
    class PasswordGenerator;
}
```

#### Constructors

```cpp
explicit PasswordGenerator(const config::PasswordGeneratorConfig& cfg = {});
```

Creates a password generator with the specified configuration.

**Parameters:**
- `cfg`: Configuration object (optional, uses defaults if not provided)

#### Methods

##### generate()
```cpp
std::string generate();
```
Generates a single password according to the current configuration.

**Returns:** Generated password string
**Throws:** `std::runtime_error` if generation fails after maximum attempts

##### generateBatch()
```cpp
std::vector<std::string> generateBatch(size_t count);
```
Generates multiple passwords.

**Parameters:**
- `count`: Number of passwords to generate

**Returns:** Vector of generated password strings

##### validatePassword()
```cpp
bool validatePassword(const std::string& password) const;
```
Validates a password against current validation rules.

**Parameters:**
- `password`: Password to validate

**Returns:** `true` if password is valid, `false` otherwise

##### getValidationErrors()
```cpp
std::vector<std::string> getValidationErrors(const std::string& password) const;
```
Gets detailed validation error messages for a password.

**Parameters:**
- `password`: Password to validate

**Returns:** Vector of error messages (empty if valid)

##### setConfig()
```cpp
void setConfig(const config::PasswordGeneratorConfig& cfg);
```
Updates the configuration.

**Parameters:**
- `cfg`: New configuration

##### getConfig()
```cpp
const config::PasswordGeneratorConfig& getConfig() const;
```
Gets the current configuration.

**Returns:** Reference to current configuration

##### addValidator()
```cpp
void addValidator(std::unique_ptr<interfaces::IPasswordValidator> validator);
```
Adds a custom validator to the validation pipeline.

**Parameters:**
- `validator`: Custom validator instance

##### clearValidators()
```cpp
void clearValidators();
```
Removes all validators from the validation pipeline.

### PasswordGeneratorConfig

Configuration structure for password generation.

```cpp
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator::core::config {
    struct PasswordGeneratorConfig;
}
```

#### Fields

```cpp
struct PasswordGeneratorConfig {
    size_t length = 16;                    // Password length
    bool includeLowercase = true;          // Include a-z
    bool includeUppercase = true;          // Include A-Z
    bool includeDigits = true;             // Include 0-9
    bool includeSymbols = true;            // Include symbols
    std::string customSymbols = "!@#$%^&*()_+-=[]{}|;:,.<>?";  // Symbol set
    bool pronounceable = false;            // Use pronounceable generation
    
    // Validation settings
    size_t minLength = 8;                  // Minimum allowed length
    size_t maxLength = 128;                // Maximum allowed length
    bool requireMixedCase = true;          // Require both upper and lower
    bool requireDigits = true;             // Require at least one digit
    bool requireSymbols = false;           // Require at least one symbol
};
```

## Interfaces

### IPasswordStrategy

Interface for password generation strategies.

```cpp
#include "core/interfaces/IPasswordStrategy.h"

namespace password_generator::core::interfaces {
    class IPasswordStrategy;
}
```

#### Methods

```cpp
virtual std::string generate(size_t length) = 0;
```
Generate a password of specified length.

**Parameters:**
- `length`: Desired password length

**Returns:** Generated password string

### IPasswordValidator

Interface for password validation rules.

```cpp
#include "core/interfaces/IPasswordValidator.h"

namespace password_generator::core::interfaces {
    class IPasswordValidator;
}
```

#### Methods

```cpp
virtual bool validate(const std::string& password) const = 0;
```
Validate a password against this rule.

**Parameters:**
- `password`: Password to validate

**Returns:** `true` if valid, `false` otherwise

```cpp
virtual std::string getErrorMessage() const = 0;
```
Get error message for validation failure.

**Returns:** Human-readable error message

### ICharacterSetProvider

Interface for character set providers.

```cpp
#include "core/interfaces/ICharacterSetProvider.h"

namespace password_generator::core::interfaces {
    class ICharacterSetProvider;
}
```

#### Methods

```cpp
virtual std::string getCharacters() const = 0;
```
Get the character set.

**Returns:** String containing all available characters

```cpp
virtual std::string getName() const = 0;
```
Get the name of this character set.

**Returns:** Descriptive name

### IRandomGenerator

Interface for random number generation.

```cpp
#include "core/interfaces/IRandomGenerator.h"

namespace password_generator::core::interfaces {
    class IRandomGenerator;
}
```

#### Methods

```cpp
virtual int generate(int min, int max) = 0;
```
Generate a random integer in the specified range.

**Parameters:**
- `min`: Minimum value (inclusive)
- `max`: Maximum value (inclusive)

**Returns:** Random integer in [min, max]

## Strategies

### StandardPasswordStrategy

Standard password generation using character sets.

```cpp
#include "strategies/StandardPasswordStrategy.h"

namespace password_generator::strategies {
    class StandardPasswordStrategy : public core::interfaces::IPasswordStrategy;
}
```

#### Constructor

```cpp
explicit StandardPasswordStrategy(
    std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
```

**Parameters:**
- `randomGen`: Custom random generator (optional, uses secure default)

#### Methods

```cpp
void addCharacterSet(std::unique_ptr<core::interfaces::ICharacterSetProvider> provider);
```
Add a character set to use for generation.

```cpp
void clearCharacterSets();
```
Remove all character sets.

```cpp
std::string generate(size_t length) override;
```
Generate password using configured character sets.

### PronounceablePasswordStrategy

Generates pronounceable passwords using syllable patterns.

```cpp
#include "strategies/PronounceablePasswordStrategy.h"

namespace password_generator::strategies {
    class PronounceablePasswordStrategy : public core::interfaces::IPasswordStrategy;
}
```

#### Constructor

```cpp
explicit PronounceablePasswordStrategy(
    std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
```

#### Methods

```cpp
void setIncludeNumbers(bool include);
```
Enable/disable number inclusion.

```cpp
void setIncludeCapitals(bool include);
```
Enable/disable capital letters.

```cpp
std::string generate(size_t length) override;
```
Generate pronounceable password.

### PatternPasswordStrategy

Generates passwords based on patterns.

```cpp
#include "strategies/PatternPasswordStrategy.h"

namespace password_generator::strategies {
    class PatternPasswordStrategy : public core::interfaces::IPasswordStrategy;
}
```

#### Constructor

```cpp
explicit PatternPasswordStrategy(
    const std::string& pattern,
    std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
```

**Parameters:**
- `pattern`: Pattern string (L=lowercase, U=uppercase, D=digit, S=symbol)

#### Methods

```cpp
void setPattern(const std::string& pattern);
```
Set the generation pattern.

```cpp
const std::string& getPattern() const;
```
Get the current pattern.

```cpp
std::string generate(size_t length) override;
```
Generate password following the pattern.

#### Pattern Format

- `L`: Lowercase letter (a-z)
- `U`: Uppercase letter (A-Z)
- `D`: Digit (0-9)
- `S`: Symbol
- Other characters: Used literally

**Examples:**
- `"LLDDSS"` � `"ab12@#"`
- `"ULL-DDD"` � `"Abc-123"`
- `"pass-DDDD"` � `"pass-5678"`

## Validators

### MinLengthValidator

Validates minimum password length.

```cpp
#include "validators/MinLengthValidator.h"

namespace password_generator::validators {
    class MinLengthValidator : public core::interfaces::IPasswordValidator;
}
```

#### Constructor

```cpp
explicit MinLengthValidator(size_t minLength);
```

#### Methods

```cpp
void setMinLength(size_t length);
size_t getMinLength() const;
```

### MaxLengthValidator

Validates maximum password length.

```cpp
#include "validators/MaxLengthValidator.h"

namespace password_generator::validators {
    class MaxLengthValidator : public core::interfaces::IPasswordValidator;
}
```

#### Constructor

```cpp
explicit MaxLengthValidator(size_t maxLength);
```

#### Methods

```cpp
void setMaxLength(size_t length);
size_t getMaxLength() const;
```

### CharacterTypeValidator

Validates required character types.

```cpp
#include "validators/CharacterTypeValidator.h"

namespace password_generator::validators {
    class CharacterTypeValidator : public core::interfaces::IPasswordValidator;
}
```

#### Constructor

```cpp
CharacterTypeValidator(bool requireUpper, bool requireLower, 
                      bool requireDigit, bool requireSymbol);
```

### EntropyValidator

Validates password entropy (randomness).

```cpp
#include "validators/EntropyValidator.h"

namespace password_generator::validators {
    class EntropyValidator : public core::interfaces::IPasswordValidator;
}
```

#### Constructor

```cpp
explicit EntropyValidator(double minEntropy);
```

**Parameters:**
- `minEntropy`: Minimum entropy in bits

#### Methods

```cpp
void setMinEntropy(double entropy);
double getMinEntropy() const;
```

## Character Set Providers

### LowercaseProvider

Provides lowercase letters (a-z).

```cpp
#include "providers/LowercaseProvider.h"

namespace password_generator::providers {
    class LowercaseProvider : public core::interfaces::ICharacterSetProvider;
}
```

### UppercaseProvider

Provides uppercase letters (A-Z).

```cpp
#include "providers/UppercaseProvider.h"

namespace password_generator::providers {
    class UppercaseProvider : public core::interfaces::ICharacterSetProvider;
}
```

### DigitProvider

Provides digits (0-9).

```cpp
#include "providers/DigitProvider.h"

namespace password_generator::providers {
    class DigitProvider : public core::interfaces::ICharacterSetProvider;
}
```

### SymbolProvider

Provides symbols with customizable character set.

```cpp
#include "providers/SymbolProvider.h"

namespace password_generator::providers {
    class SymbolProvider : public core::interfaces::ICharacterSetProvider;
}
```

#### Constructor

```cpp
explicit SymbolProvider(const std::string& symbols = "!@#$%^&*()_+-=[]{}|;:,.<>?");
```

**Parameters:**
- `symbols`: Custom symbol set (optional)

#### Methods

```cpp
void setSymbols(const std::string& symbols);
const std::string& getSymbols() const;
```

## Utilities

### SecureRandomGenerator

Cryptographically secure random number generator.

```cpp
#include "utils/SecureRandomGenerator.h"

namespace password_generator::utils {
    class SecureRandomGenerator : public core::interfaces::IRandomGenerator;
}
```

#### Methods

```cpp
int generate(int min, int max) override;
```
Generate secure random integer.

```cpp
void reseed();
```
Re-seed the generator.

## CLI Interface

### PasswordGeneratorCLI

Command-line interface for the password generator.

```cpp
#include "cli/PasswordGeneratorCLI.h"

namespace password_generator::cli {
    class PasswordGeneratorCLI;
}
```

#### Methods

```cpp
int processArgs(int argc, char* argv[]);
```
Process command-line arguments for automated access. Returns exit code.

#### Command-Line Options

- `-h, --help`: Show help message
- `--version`: Show version information
- `-g, --generate`: Generate a single password
- `-b, --batch <count>`: Generate multiple passwords
- `-l, --length <n>`: Set password length (8-128)
- `--no-lowercase`: Exclude lowercase characters
- `--no-uppercase`: Exclude uppercase characters
- `--no-digits`: Exclude digit characters
- `--no-symbols`: Exclude symbol characters
- `-s, --symbols <chars>`: Set custom symbol set
- `-p, --pronounceable`: Generate pronounceable password
- `-c, --config`: Show current configuration
- `-v, --validate <pass>`: Validate a password
- `-q, --quiet`: Suppress prompts and decorations

## Example Usage

### Basic Usage

```cpp
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"

using namespace password_generator::core;

int main() {
    // Create configuration
    config::PasswordGeneratorConfig config;
    config.length = 20;
    config.includeSymbols = true;
    
    // Create generator
    PasswordGenerator generator(config);
    
    // Generate password
    std::string password = generator.generate();
    std::cout << "Generated: " << password << std::endl;
    
    // Validate password
    if (generator.validatePassword(password)) {
        std::cout << "Password is valid" << std::endl;
    }
    
    return 0;
}
```

### Custom Strategy

```cpp
#include "strategies/PatternPasswordStrategy.h"

auto strategy = std::make_unique<PatternPasswordStrategy>("ULL-DDD-SS");
std::string password = strategy->generate(10); // "Abc-123-@#"
```

### Custom Validator

```cpp
#include "validators/EntropyValidator.h"

PasswordGenerator generator;
generator.addValidator(std::make_unique<EntropyValidator>(60.0)); // 60-bit minimum
```

### Batch Generation

```cpp
auto passwords = generator.generateBatch(100);
for (const auto& pwd : passwords) {
    std::cout << pwd << std::endl;
}
```