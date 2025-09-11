# Extending the Password Generator

This guide explains how to extend the password generator library with custom strategies, validators, character sets, and other components.

## Overview

The password generator is designed with extensibility as a core principle. All major components use interfaces that can be implemented by custom classes, allowing you to:

- Create new password generation algorithms
- Add custom validation rules
- Define new character sets
- Implement alternative random generators
- Build custom CLI commands

## Creating Custom Password Strategies

### Step 1: Implement IPasswordStrategy

Create a new class that implements the `IPasswordStrategy` interface:

```cpp
#include "core/interfaces/IPasswordStrategy.h"
#include "core/interfaces/IRandomGenerator.h"
#include "utils/SecureRandomGenerator.h"

namespace password_generator::strategies {

class WordListPasswordStrategy : public core::interfaces::IPasswordStrategy {
public:
    explicit WordListPasswordStrategy(
        const std::vector<std::string>& words,
        const std::string& separator = "-",
        std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr)
        : words_(words), separator_(separator), 
          rng_(randomGen ? std::move(randomGen) 
               : std::make_unique<utils::SecureRandomGenerator>()) {}
    
    std::string generate(size_t length) override {
        std::string password;
        while (password.length() < length) {
            if (!password.empty()) {
                password += separator_;
            }
            
            // Select random word
            size_t index = rng_->generate(0, words_.size() - 1);
            password += words_[index];
            
            // Add random number suffix
            if (password.length() < length - 2) {
                password += std::to_string(rng_->generate(10, 99));
            }
        }
        
        // Trim to exact length if needed
        if (password.length() > length) {
            password.resize(length);
        }
        
        return password;
    }

private:
    std::vector<std::string> words_;
    std::string separator_;
    std::unique_ptr<core::interfaces::IRandomGenerator> rng_;
};

} // namespace password_generator::strategies
```

### Step 2: Create Header File

```cpp
// include/strategies/WordListPasswordStrategy.h
#ifndef WORD_LIST_PASSWORD_STRATEGY_H
#define WORD_LIST_PASSWORD_STRATEGY_H

#include "core/interfaces/IPasswordStrategy.h"
#include "core/interfaces/IRandomGenerator.h"
#include <vector>
#include <string>
#include <memory>

namespace password_generator {
namespace strategies {

class WordListPasswordStrategy : public core::interfaces::IPasswordStrategy {
public:
    explicit WordListPasswordStrategy(
        const std::vector<std::string>& words,
        const std::string& separator = "-",
        std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
    
    std::string generate(size_t length) override;
    
    void setWords(const std::vector<std::string>& words);
    void setSeparator(const std::string& separator);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace strategies
} // namespace password_generator

#endif // WORD_LIST_PASSWORD_STRATEGY_H
```

### Step 3: Write Tests

```cpp
// tests/unit/strategies/WordListPasswordStrategyTest.cpp
#include <gtest/gtest.h>
#include "strategies/WordListPasswordStrategy.h"
#include "mocks/MockRandomGenerator.h"

using namespace password_generator::strategies;
using namespace password_generator::tests;

TEST(WordListPasswordStrategyTest, GeneratesFromWordList) {
    std::vector<std::string> words = {"hello", "world", "test"};
    auto mockRng = std::make_unique<MockRandomGenerator>(
        std::vector<int>{0, 42, 1, 78}); // word indices and numbers
    
    WordListPasswordStrategy strategy(words, "-", std::move(mockRng));
    std::string password = strategy.generate(20);
    
    EXPECT_EQ(password, "hello42-world78");
}
```

## Creating Custom Validators

### Step 1: Implement IPasswordValidator

```cpp
#include "core/interfaces/IPasswordValidator.h"
#include <regex>

namespace password_generator::validators {

class RegexValidator : public core::interfaces::IPasswordValidator {
public:
    explicit RegexValidator(const std::string& pattern, 
                           const std::string& errorMessage)
        : regex_(pattern), errorMessage_(errorMessage) {}
    
    bool validate(const std::string& password) const override {
        return std::regex_match(password, regex_);
    }
    
    std::string getErrorMessage() const override {
        return errorMessage_;
    }
    
    void setPattern(const std::string& pattern) {
        regex_ = std::regex(pattern);
    }

private:
    std::regex regex_;
    std::string errorMessage_;
};

// Example: No consecutive identical characters
class NoConsecutiveValidator : public core::interfaces::IPasswordValidator {
public:
    bool validate(const std::string& password) const override {
        for (size_t i = 1; i < password.length(); ++i) {
            if (password[i] == password[i-1]) {
                return false;
            }
        }
        return true;
    }
    
    std::string getErrorMessage() const override {
        return "Password cannot contain consecutive identical characters";
    }
};

} // namespace password_generator::validators
```

### Step 2: Usage Example

```cpp
#include "validators/RegexValidator.h"
#include "validators/NoConsecutiveValidator.h"
#include "core/PasswordGenerator.h"

using namespace password_generator;

int main() {
    core::PasswordGenerator generator;
    
    // Add custom validators
    generator.addValidator(
        std::make_unique<validators::RegexValidator>(
            "^(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9]).*$",
            "Password must contain uppercase, lowercase, and digit"));
    
    generator.addValidator(
        std::make_unique<validators::NoConsecutiveValidator>());
    
    // Generate with custom validation
    std::string password = generator.generate();
    
    return 0;
}
```

## Creating Custom Character Set Providers

### Step 1: Implement ICharacterSetProvider

```cpp
#include "core/interfaces/ICharacterSetProvider.h"

namespace password_generator::providers {

class ExtendedSymbolProvider : public core::interfaces::ICharacterSetProvider {
public:
    explicit ExtendedSymbolProvider(bool includeUnicode = false)
        : includeUnicode_(includeUnicode) {}
    
    std::string getCharacters() const override {
        std::string chars = "!@#$%^&*()_+-=[]{}|;:,.<>?";
        if (includeUnicode_) {
            chars += ""æ#CLMNOPQ"; // Unicode symbols
        }
        return chars;
    }
    
    std::string getName() const override {
        return includeUnicode_ ? "Extended Unicode Symbols" : "Extended Symbols";
    }

private:
    bool includeUnicode_;
};

class HexProvider : public core::interfaces::ICharacterSetProvider {
public:
    explicit HexProvider(bool uppercase = false)
        : uppercase_(uppercase) {}
    
    std::string getCharacters() const override {
        return uppercase_ ? "0123456789ABCDEF" : "0123456789abcdef";
    }
    
    std::string getName() const override {
        return uppercase_ ? "Uppercase Hexadecimal" : "Lowercase Hexadecimal";
    }

private:
    bool uppercase_;
};

} // namespace password_generator::providers
```

### Step 2: Usage with StandardPasswordStrategy

```cpp
#include "strategies/StandardPasswordStrategy.h"
#include "providers/ExtendedSymbolProvider.h"
#include "providers/HexProvider.h"

auto strategy = std::make_unique<StandardPasswordStrategy>();
strategy->addCharacterSet(std::make_unique<ExtendedSymbolProvider>(true));
strategy->addCharacterSet(std::make_unique<HexProvider>(false));

std::string password = strategy->generate(16);
```

## Creating Custom Random Generators

### Step 1: Implement IRandomGenerator

```cpp
#include "core/interfaces/IRandomGenerator.h"
#include <random>

namespace password_generator::utils {

class PseudoRandomGenerator : public core::interfaces::IRandomGenerator {
public:
    explicit PseudoRandomGenerator(unsigned int seed)
        : gen_(seed) {}
    
    int generate(int min, int max) override {
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen_);
    }
    
    void setSeed(unsigned int seed) {
        gen_.seed(seed);
    }

private:
    std::mt19937 gen_;
};

// For testing - generates predictable sequence
class SequenceGenerator : public core::interfaces::IRandomGenerator {
public:
    explicit SequenceGenerator(int start = 0, int step = 1)
        : current_(start), step_(step) {}
    
    int generate(int min, int max) override {
        int value = std::clamp(current_, min, max);
        current_ += step_;
        return value;
    }

private:
    int current_;
    int step_;
};

} // namespace password_generator::utils
```

## Advanced Extensions

### Custom Configuration Types

You can extend the configuration system:

```cpp
struct AdvancedPasswordConfig : public core::config::PasswordGeneratorConfig {
    bool includeEmoji = false;
    std::string wordListPath;
    int minWordLength = 3;
    int maxWordLength = 8;
    bool includeNumbers = true;
    std::string numberFormat = "suffix"; // "suffix", "prefix", "middle"
};
```

### Plugin Architecture

For dynamic loading of extensions:

```cpp
// Extension interface
class IPasswordGeneratorExtension {
public:
    virtual ~IPasswordGeneratorExtension() = default;
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual void registerComponents(PasswordGeneratorRegistry& registry) = 0;
};

// Plugin loader
class PluginLoader {
public:
    void loadPlugin(const std::string& path) {
        // Dynamic library loading logic
        #ifdef _WIN32
            HMODULE handle = LoadLibrary(path.c_str());
        #else
            void* handle = dlopen(path.c_str(), RTLD_LAZY);
        #endif
        
        if (handle) {
            auto createExtension = reinterpret_cast<IPasswordGeneratorExtension*(*)()>(
                #ifdef _WIN32
                    GetProcAddress(handle, "createExtension")
                #else
                    dlsym(handle, "createExtension")
                #endif
            );
            
            if (createExtension) {
                extensions_.push_back(std::unique_ptr<IPasswordGeneratorExtension>(
                    createExtension()));
            }
        }
    }

private:
    std::vector<std::unique_ptr<IPasswordGeneratorExtension>> extensions_;
};
```

### Custom CLI Commands

Extend the CLI with custom commands:

```cpp
class CustomPasswordGeneratorCLI : public cli::PasswordGeneratorCLI {
public:
    CustomPasswordGeneratorCLI() {
        addCommand("wordlist", [this]() { generateFromWordList(); });
        addCommand("pattern", [this]() { generateFromPattern(); });
        addCommand("analyze", [this]() { analyzePassword(); });
    }

private:
    void generateFromWordList() {
        std::cout << "Enter word list file: ";
        std::string filename;
        std::getline(std::cin, filename);
        
        // Load word list and generate
        auto words = loadWordList(filename);
        auto strategy = std::make_unique<WordListPasswordStrategy>(words);
        std::string password = strategy->generate(16);
        
        std::cout << "Generated: " << password << std::endl;
    }
    
    void generateFromPattern() {
        std::cout << "Enter pattern (L=lower, U=upper, D=digit, S=symbol): ";
        std::string pattern;
        std::getline(std::cin, pattern);
        
        auto strategy = std::make_unique<PatternPasswordStrategy>(pattern);
        std::string password = strategy->generate(pattern.length());
        
        std::cout << "Generated: " << password << std::endl;
    }
    
    void analyzePassword() {
        std::cout << "Enter password to analyze: ";
        std::string password;
        std::getline(std::cin, password);
        
        // Perform analysis
        double entropy = calculateEntropy(password);
        int strength = calculateStrength(password);
        
        std::cout << "Analysis Results:" << std::endl;
        std::cout << "  Length: " << password.length() << std::endl;
        std::cout << "  Entropy: " << entropy << " bits" << std::endl;
        std::cout << "  Strength: " << strength << "/10" << std::endl;
    }
};
```

## Testing Extensions

### Unit Testing

Always create comprehensive unit tests for extensions:

```cpp
class CustomStrategyTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockRng_ = std::make_unique<MockRandomGenerator>(
            std::vector<int>{0, 1, 2, 3, 4, 5});
        strategy_ = std::make_unique<CustomStrategy>(std::move(mockRng_));
    }
    
    std::unique_ptr<CustomStrategy> strategy_;
    std::unique_ptr<MockRandomGenerator> mockRng_;
};

TEST_F(CustomStrategyTest, GeneratesExpectedOutput) {
    std::string password = strategy_->generate(10);
    EXPECT_EQ(password.length(), 10);
    // Add specific assertions for your strategy
}
```

### Integration Testing

Test how extensions work with the main system:

```cpp
TEST(ExtensionIntegrationTest, CustomStrategyWithPasswordGenerator) {
    core::PasswordGenerator generator;
    
    // Create custom strategy
    auto strategy = std::make_unique<CustomStrategy>();
    
    // Test integration (this would require extending PasswordGenerator
    // to accept custom strategies)
    std::string password = strategy->generate(16);
    
    EXPECT_TRUE(generator.validatePassword(password));
}
```

## Best Practices

### 1. Follow SOLID Principles
- **Single Responsibility**: Each class should have one reason to change
- **Open/Closed**: Open for extension, closed for modification
- **Liskov Substitution**: Extensions should be substitutable for base types
- **Interface Segregation**: Use focused interfaces
- **Dependency Inversion**: Depend on abstractions, not concretions

### 2. Use RAII and Smart Pointers
```cpp
class CustomStrategy {
private:
    std::unique_ptr<ResourceType> resource_;  // Automatic cleanup
    std::vector<std::unique_ptr<ComponentType>> components_;
};
```

### 3. Provide Clear Error Messages
```cpp
std::string getErrorMessage() const override {
    return "Password must contain at least " + std::to_string(minWords_) + 
           " words from the approved word list";
}
```

### 4. Document Your Extensions
```cpp
/**
 * @brief Word list-based password generation strategy
 * 
 * Generates passwords by combining random words from a predefined list
 * with optional separators and numeric suffixes.
 * 
 * Example: "cat-dog-42" or "house.tree.17"
 * 
 * @note Words should be 3-8 characters for optimal security/usability balance
 */
class WordListPasswordStrategy : public core::interfaces::IPasswordStrategy {
    // ...
};
```

### 5. Consider Thread Safety
```cpp
class ThreadSafeExtension {
private:
    mutable std::shared_mutex mutex_;
    
public:
    std::string getValue() const {
        std::shared_lock lock(mutex_);
        return value_;
    }
    
    void setValue(const std::string& value) {
        std::unique_lock lock(mutex_);
        value_ = value;
    }
};
```

## Building and Packaging Extensions

### CMake Integration

Add your extensions to the build system:

```cmake
# Add extension library
add_library(password_generator_extensions STATIC
    src/extensions/WordListPasswordStrategy.cpp
    src/extensions/RegexValidator.cpp
    src/extensions/ExtendedSymbolProvider.cpp
)

target_link_libraries(password_generator_extensions 
    password_generator_lib)

# Add extension tests
add_executable(extension_tests
    tests/extensions/WordListStrategyTest.cpp
    tests/extensions/RegexValidatorTest.cpp
)

target_link_libraries(extension_tests
    password_generator_extensions
    gtest gtest_main)
```

### Installation

```cmake
# Install extension headers
install(DIRECTORY include/extensions/ 
        DESTINATION include/password_generator/extensions)

# Install extension library
install(TARGETS password_generator_extensions 
        DESTINATION lib)
```

This comprehensive extending guide provides everything needed to create powerful, maintainable extensions to the password generator library while following best practices and maintaining consistency with the existing architecture.