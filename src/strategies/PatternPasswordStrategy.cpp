#include "strategies/PatternPasswordStrategy.h"
#include "utils/SecureRandomGenerator.h"
#include <stdexcept>

namespace password_generator {
namespace strategies {

class PatternPasswordStrategy::Impl {
public:
    std::string pattern;
    std::unique_ptr<core::interfaces::IRandomGenerator> rng;
    
    static const std::string LOWERCASE_CHARS;
    static const std::string UPPERCASE_CHARS;
    static const std::string DIGIT_CHARS;
    static const std::string SYMBOL_CHARS;
    
    Impl(const std::string& pat, std::unique_ptr<core::interfaces::IRandomGenerator> randomGen)
        : pattern(pat), rng(randomGen ? std::move(randomGen) 
              : std::make_unique<utils::SecureRandomGenerator>()) {}
              
    char generateCharForType(char patternChar) {
        switch (patternChar) {
            case 'L':
                return LOWERCASE_CHARS[rng->generate(0, LOWERCASE_CHARS.length() - 1)];
            case 'U':
                return UPPERCASE_CHARS[rng->generate(0, UPPERCASE_CHARS.length() - 1)];
            case 'D':
                return DIGIT_CHARS[rng->generate(0, DIGIT_CHARS.length() - 1)];
            case 'S':
                return SYMBOL_CHARS[rng->generate(0, SYMBOL_CHARS.length() - 1)];
            default:
                return patternChar; // Use literally
        }
    }
};

const std::string PatternPasswordStrategy::Impl::LOWERCASE_CHARS = "abcdefghijklmnopqrstuvwxyz";
const std::string PatternPasswordStrategy::Impl::UPPERCASE_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string PatternPasswordStrategy::Impl::DIGIT_CHARS = "0123456789";
const std::string PatternPasswordStrategy::Impl::SYMBOL_CHARS = "!@#$%^&*()_+-=[]{}|;:,.<>?";

PatternPasswordStrategy::PatternPasswordStrategy(
    const std::string& pattern,
    std::unique_ptr<core::interfaces::IRandomGenerator> randomGen)
    : pImpl(std::make_unique<Impl>(pattern, std::move(randomGen))) {}

PatternPasswordStrategy::~PatternPasswordStrategy() = default;

void PatternPasswordStrategy::setPattern(const std::string& pattern) {
    pImpl->pattern = pattern;
}

const std::string& PatternPasswordStrategy::getPattern() const {
    return pImpl->pattern;
}

std::string PatternPasswordStrategy::generate(size_t length) {
    if (pImpl->pattern.empty()) {
        throw std::runtime_error("Pattern cannot be empty");
    }
    
    std::string password;
    password.reserve(length);
    
    // Generate based on pattern, repeating pattern if necessary
    for (size_t i = 0; i < length; ++i) {
        char patternChar = pImpl->pattern[i % pImpl->pattern.length()];
        password += pImpl->generateCharForType(patternChar);
    }
    
    return password;
}

} // namespace strategies
} // namespace password_generator