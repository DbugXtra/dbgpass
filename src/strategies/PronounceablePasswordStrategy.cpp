#include "strategies/PronounceablePasswordStrategy.h"
#include "utils/SecureRandomGenerator.h"
#include <vector>
#include <cctype>

namespace password_generator {
namespace strategies {

class PronounceablePasswordStrategy::Impl {
public:
    std::unique_ptr<core::interfaces::IRandomGenerator> rng;
    bool includeNumbers = true;
    bool includeCapitals = true;
    
    const std::vector<std::string> syllables = {
        "ba", "be", "bi", "bo", "bu", "ca", "ce", "ci", "co", "cu",
        "da", "de", "di", "do", "du", "fa", "fe", "fi", "fo", "fu",
        "ga", "ge", "gi", "go", "gu", "ha", "he", "hi", "ho", "hu",
        "ja", "je", "ji", "jo", "ju", "ka", "ke", "ki", "ko", "ku",
        "la", "le", "li", "lo", "lu", "ma", "me", "mi", "mo", "mu",
        "na", "ne", "ni", "no", "nu", "pa", "pe", "pi", "po", "pu",
        "ra", "re", "ri", "ro", "ru", "sa", "se", "si", "so", "su",
        "ta", "te", "ti", "to", "tu", "va", "ve", "vi", "vo", "vu",
        "wa", "we", "wi", "wo", "wu", "ya", "ye", "yi", "yo", "yu",
        "za", "ze", "zi", "zo", "zu"
    };
    
    Impl(std::unique_ptr<core::interfaces::IRandomGenerator> randomGen)
        : rng(randomGen ? std::move(randomGen) 
              : std::make_unique<utils::SecureRandomGenerator>()) {}
};

PronounceablePasswordStrategy::PronounceablePasswordStrategy(
    std::unique_ptr<core::interfaces::IRandomGenerator> randomGen)
    : pImpl(std::make_unique<Impl>(std::move(randomGen))) {}

PronounceablePasswordStrategy::~PronounceablePasswordStrategy() = default;

void PronounceablePasswordStrategy::setIncludeNumbers(bool include) {
    pImpl->includeNumbers = include;
}

void PronounceablePasswordStrategy::setIncludeCapitals(bool include) {
    pImpl->includeCapitals = include;
}

std::string PronounceablePasswordStrategy::generate(size_t length) {
    std::string password;
    
    while (password.length() < length) {
        // Add syllable
        const std::string& syllable = 
            pImpl->syllables[pImpl->rng->generate(0, pImpl->syllables.size() - 1)];
        password += syllable;
        
        // Randomly capitalize
        if (pImpl->includeCapitals && pImpl->rng->generate(0, 2) == 0 && 
            password.length() >= 2) {
            password[password.length() - 2] = 
                std::toupper(password[password.length() - 2]);
        }
        
        // Occasionally add number
        if (pImpl->includeNumbers && pImpl->rng->generate(0, 3) == 0 && 
            password.length() < length) {
            password += std::to_string(pImpl->rng->generate(0, 9));
        }
    }
    
    // Trim to exact length
    if (password.length() > length) {
        password = password.substr(0, length);
    }
    
    return password;
}

} // namespace strategies
} // namespace password_generator