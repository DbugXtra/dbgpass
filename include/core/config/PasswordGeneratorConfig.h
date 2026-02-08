#pragma once

#include <string>
#include <cstddef>

namespace password_generator {
namespace core {
namespace config {

/**
 * @brief Configuration structure for the password generator
 */
struct PasswordGeneratorConfig {
    // Generation settings
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
    double minEntropy = 50.0; // Minimum entropy in bits
};

} // namespace config
} // namespace core
} // namespace password_generator
