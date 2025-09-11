# Examples

This document provides comprehensive examples of how to use the Secure Password Generator library.

## Basic Usage

### Simple Password Generation

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>

int main() {
    using namespace password_generator::core;
    
    // Create generator with default settings
    PasswordGenerator generator;
    
    // Generate a password
    std::string password = generator.generate();
    std::cout << "Generated password: " << password << std::endl;
    
    return 0;
}
```

### Custom Configuration

```cpp
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include <iostream>

int main() {
    using namespace password_generator::core;
    
    // Create custom configuration
    config::PasswordGeneratorConfig config;
    config.length = 24;                    // Longer password
    config.includeSymbols = true;          // Include symbols
    config.requireSymbols = true;          // Require at least one symbol
    config.customSymbols = "@#$%&*!";      // Custom symbol set
    
    // Create generator with configuration
    PasswordGenerator generator(config);
    
    // Generate password
    std::string password = generator.generate();
    std::cout << "Generated password: " << password << std::endl;
    
    return 0;
}
```

## Batch Generation

### Generate Multiple Passwords

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>

int main() {
    using namespace password_generator::core;
    
    PasswordGenerator generator;
    
    // Generate 10 passwords
    auto passwords = generator.generateBatch(10);
    
    std::cout << "Generated passwords:" << std::endl;
    for (size_t i = 0; i < passwords.size(); ++i) {
        std::cout << (i + 1) << ". " << passwords[i] << std::endl;
    }
    
    return 0;
}
```

### Batch with Different Configurations

```cpp
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include <iostream>
#include <vector>

int main() {
    using namespace password_generator::core;
    
    // Define different configurations
    std::vector<config::PasswordGeneratorConfig> configs = {
        {16, true, true, true, false},  // No symbols
        {12, true, true, false, true},  // No digits
        {20, false, true, true, true}   // No lowercase
    };
    
    for (size_t i = 0; i < configs.size(); ++i) {
        PasswordGenerator generator(configs[i]);
        auto passwords = generator.generateBatch(3);
        
        std::cout << "Configuration " << (i + 1) << " passwords:" << std::endl;
        for (const auto& pwd : passwords) {
            std::cout << "  " << pwd << std::endl;
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```

## Using Different Strategies

### Standard Strategy with Custom Character Sets

```cpp
#include "strategies/StandardPasswordStrategy.h"
#include "providers/LowercaseProvider.h"
#include "providers/DigitProvider.h"
#include <iostream>

int main() {
    using namespace password_generator::strategies;
    using namespace password_generator::providers;
    
    // Create strategy
    auto strategy = std::make_unique<StandardPasswordStrategy>();
    
    // Add only specific character sets
    strategy->addCharacterSet(std::make_unique<LowercaseProvider>());
    strategy->addCharacterSet(std::make_unique<DigitProvider>());
    
    // Generate password
    std::string password = strategy->generate(16);
    std::cout << "Lowercase + digits only: " << password << std::endl;
    
    return 0;
}
```

### Pronounceable Passwords

```cpp
#include "strategies/PronounceablePasswordStrategy.h"
#include <iostream>

int main() {
    using namespace password_generator::strategies;
    
    // Create pronounceable strategy
    auto strategy = std::make_unique<PronounceablePasswordStrategy>();
    
    // Configure options
    strategy->setIncludeNumbers(true);
    strategy->setIncludeCapitals(true);
    
    // Generate pronounceable password
    std::string password = strategy->generate(16);
    std::cout << "Pronounceable password: " << password << std::endl;
    
    return 0;
}
```

### Pattern-Based Generation

```cpp
#include "strategies/PatternPasswordStrategy.h"
#include <iostream>

int main() {
    using namespace password_generator::strategies;
    
    // Different patterns
    std::vector<std::string> patterns = {
        "ULL-DDD-SS",           // Abc-123-@#
        "DDDD-LLLL-DDDD",       // 1234-abcd-5678
        "pass-DDDD",            // pass-1234
        "ULULULUL",             // AbCdEfGh
        "LL.LL.DD"              // ab.cd.12
    };
    
    for (const auto& pattern : patterns) {
        auto strategy = std::make_unique<PatternPasswordStrategy>(pattern);
        std::string password = strategy->generate(pattern.length());
        
        std::cout << "Pattern '" << pattern << "' → " << password << std::endl;
    }
    
    return 0;
}
```

## Password Validation

### Basic Validation

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>
#include <vector>

int main() {
    using namespace password_generator::core;
    
    PasswordGenerator generator;
    
    // Test passwords
    std::vector<std::string> testPasswords = {
        "password123",          // Common password
        "P@ssw0rd!23",         // Strong password
        "abc",                  // Too short
        "verylongpasswordwithoutuppercasedigitorsymbol"  // No variety
    };
    
    for (const auto& pwd : testPasswords) {
        bool isValid = generator.validatePassword(pwd);
        auto errors = generator.getValidationErrors(pwd);
        
        std::cout << "Password: " << pwd << std::endl;
        std::cout << "Valid: " << (isValid ? "Yes" : "No") << std::endl;
        
        if (!errors.empty()) {
            std::cout << "Errors:" << std::endl;
            for (const auto& error : errors) {
                std::cout << "  - " << error << std::endl;
            }
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```

### Custom Validators

```cpp
#include "core/PasswordGenerator.h"
#include "validators/EntropyValidator.h"
#include <iostream>

// Custom validator: No dictionary words
class NoDictionaryWordsValidator : public password_generator::core::interfaces::IPasswordValidator {
private:
    std::vector<std::string> commonWords_ = {
        "password", "123456", "qwerty", "admin", "letmein", "welcome"
    };
    
public:
    bool validate(const std::string& password) const override {
        std::string lowerPassword = password;
        std::transform(lowerPassword.begin(), lowerPassword.end(), 
                      lowerPassword.begin(), ::tolower);
        
        for (const auto& word : commonWords_) {
            if (lowerPassword.find(word) != std::string::npos) {
                return false;
            }
        }
        return true;
    }
    
    std::string getErrorMessage() const override {
        return "Password contains common dictionary words";
    }
};

int main() {
    using namespace password_generator::core;
    using namespace password_generator::validators;
    
    PasswordGenerator generator;
    
    // Add custom validators
    generator.addValidator(std::make_unique<EntropyValidator>(50.0));  // 50-bit minimum
    generator.addValidator(std::make_unique<NoDictionaryWordsValidator>());
    
    // Generate with enhanced validation
    std::string password = generator.generate();
    std::cout << "High-entropy password: " << password << std::endl;
    
    return 0;
}
```

## Advanced Usage

### Multi-Threading Example

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex outputMutex;

void generatePasswordsInThread(int threadId, int count) {
    using namespace password_generator::core;
    
    PasswordGenerator generator;
    auto passwords = generator.generateBatch(count);
    
    std::lock_guard<std::mutex> lock(outputMutex);
    std::cout << "Thread " << threadId << " generated " << count << " passwords:" << std::endl;
    for (const auto& pwd : passwords) {
        std::cout << "  " << pwd << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    const int numThreads = 4;
    const int passwordsPerThread = 5;
    
    std::vector<std::thread> threads;
    
    // Launch threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(generatePasswordsInThread, i + 1, passwordsPerThread);
    }
    
    // Wait for completion
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "All threads completed." << std::endl;
    return 0;
}
```

### Performance Benchmark

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>
#include <chrono>

int main() {
    using namespace password_generator::core;
    using namespace std::chrono;
    
    PasswordGenerator generator;
    const int iterations = 10000;
    
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        generator.generate();
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    std::cout << "Generated " << iterations << " passwords in " 
              << duration.count() << " microseconds" << std::endl;
    std::cout << "Average: " << (duration.count() / iterations) 
              << " microseconds per password" << std::endl;
    
    return 0;
}
```

### Configuration from JSON

```cpp
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>  // External JSON library

password_generator::core::config::PasswordGeneratorConfig 
loadConfigFromJSON(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;
    
    password_generator::core::config::PasswordGeneratorConfig config;
    config.length = j.value("length", 16);
    config.includeLowercase = j.value("includeLowercase", true);
    config.includeUppercase = j.value("includeUppercase", true);
    config.includeDigits = j.value("includeDigits", true);
    config.includeSymbols = j.value("includeSymbols", true);
    config.customSymbols = j.value("customSymbols", "!@#$%^&*()_+-=[]{}|;:,.<>?");
    
    return config;
}

int main() {
    using namespace password_generator::core;
    
    try {
        // Load configuration from JSON file
        auto config = loadConfigFromJSON("password_config.json");
        
        PasswordGenerator generator(config);
        std::string password = generator.generate();
        
        std::cout << "Password from JSON config: " << password << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

Example `password_config.json`:
```json
{
    "length": 20,
    "includeLowercase": true,
    "includeUppercase": true,
    "includeDigits": true,
    "includeSymbols": true,
    "customSymbols": "@#$%&*!?",
    "requireMixedCase": true,
    "requireDigits": true,
    "requireSymbols": true
}
```

## Error Handling

### Comprehensive Error Handling

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>
#include <stdexcept>

int main() {
    using namespace password_generator::core;
    
    try {
        config::PasswordGeneratorConfig config;
        config.length = 4;  // Too short
        config.minLength = 8;
        
        PasswordGenerator generator(config);
        
        // This will throw because length < minLength
        std::string password = generator.generate();
        std::cout << "Generated: " << password << std::endl;
        
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        
        // Try with corrected config
        config::PasswordGeneratorConfig correctedConfig;
        correctedConfig.length = 12;
        
        PasswordGenerator fallbackGenerator(correctedConfig);
        std::string password = fallbackGenerator.generate();
        std::cout << "Fallback password: " << password << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Integration Examples

### Web Server Integration (with Crow framework)

```cpp
#include "crow.h"
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include <nlohmann/json.hpp>

int main() {
    using namespace password_generator::core;
    
    crow::SimpleApp app;
    
    CROW_ROUTE(app, "/generate").methods("POST"_method)
    ([](const crow::request& req) {
        nlohmann::json requestJson = nlohmann::json::parse(req.body);
        
        // Create config from request
        config::PasswordGeneratorConfig config;
        config.length = requestJson.value("length", 16);
        config.includeUppercase = requestJson.value("uppercase", true);
        config.includeLowercase = requestJson.value("lowercase", true);
        config.includeDigits = requestJson.value("digits", true);
        config.includeSymbols = requestJson.value("symbols", false);
        
        try {
            PasswordGenerator generator(config);
            std::string password = generator.generate();
            
            nlohmann::json response;
            response["password"] = password;
            response["length"] = password.length();
            response["status"] = "success";
            
            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["status"] = "error";
            error["message"] = e.what();
            
            return crow::response(400, error.dump());
        }
    });
    
    CROW_ROUTE(app, "/validate").methods("POST"_method)
    ([](const crow::request& req) {
        nlohmann::json requestJson = nlohmann::json::parse(req.body);
        std::string password = requestJson["password"];
        
        PasswordGenerator generator;
        bool isValid = generator.validatePassword(password);
        auto errors = generator.getValidationErrors(password);
        
        nlohmann::json response;
        response["valid"] = isValid;
        response["errors"] = errors;
        
        return crow::response(200, response.dump());
    });
    
    app.port(8080).multithreaded().run();
    return 0;
}
```

### Command Line Tool

```cpp
#include "core/PasswordGenerator.h"
#include <iostream>
#include <string>
#include <cstring>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << "  -l, --length LENGTH    Password length (default: 16)\n"
              << "  -c, --count COUNT      Generate COUNT passwords (default: 1)\n"
              << "  --no-upper             Exclude uppercase letters\n"
              << "  --no-lower             Exclude lowercase letters\n"
              << "  --no-digits            Exclude digits\n"
              << "  --symbols              Include symbols\n"
              << "  --custom-symbols SYMS  Use custom symbol set\n"
              << "  -h, --help             Show this help\n";
}

int main(int argc, char* argv[]) {
    using namespace password_generator::core;
    
    config::PasswordGeneratorConfig config;
    int count = 1;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--length") == 0) {
            if (i + 1 < argc) {
                config.length = std::stoul(argv[++i]);
            }
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--count") == 0) {
            if (i + 1 < argc) {
                count = std::stoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "--no-upper") == 0) {
            config.includeUppercase = false;
        } else if (strcmp(argv[i], "--no-lower") == 0) {
            config.includeLowercase = false;
        } else if (strcmp(argv[i], "--no-digits") == 0) {
            config.includeDigits = false;
        } else if (strcmp(argv[i], "--symbols") == 0) {
            config.includeSymbols = true;
        } else if (strcmp(argv[i], "--custom-symbols") == 0) {
            if (i + 1 < argc) {
                config.customSymbols = argv[++i];
                config.includeSymbols = true;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        }
    }
    
    try {
        PasswordGenerator generator(config);
        
        if (count == 1) {
            std::cout << generator.generate() << std::endl;
        } else {
            auto passwords = generator.generateBatch(count);
            for (const auto& pwd : passwords) {
                std::cout << pwd << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

These examples demonstrate the flexibility and power of the Secure Password Generator library across various use cases and integration scenarios.