#include "strategies/StandardPasswordStrategy.h"
#include "utils/SecureRandomGenerator.h"
#include <stdexcept>
#include <algorithm>

namespace password_generator {
namespace strategies {

class StandardPasswordStrategy::Impl {
public:
    std::vector<std::unique_ptr<core::interfaces::ICharacterSetProvider>> providers;
    std::unique_ptr<core::interfaces::IRandomGenerator> rng;
    
    Impl(std::unique_ptr<core::interfaces::IRandomGenerator> randomGen)
        : rng(randomGen ? std::move(randomGen) 
              : std::make_unique<utils::SecureRandomGenerator>()) {}
};

StandardPasswordStrategy::StandardPasswordStrategy(
    std::unique_ptr<core::interfaces::IRandomGenerator> randomGen)
    : pImpl(std::make_unique<Impl>(std::move(randomGen))) {}

StandardPasswordStrategy::~StandardPasswordStrategy() = default;

void StandardPasswordStrategy::addCharacterSet(
    std::unique_ptr<core::interfaces::ICharacterSetProvider> provider) {
    pImpl->providers.push_back(std::move(provider));
}

void StandardPasswordStrategy::clearCharacterSets() {
    pImpl->providers.clear();
}

std::string StandardPasswordStrategy::generate(size_t length) {
    if (pImpl->providers.empty()) {
        throw std::runtime_error("No character sets configured");
    }
    
    // Combine all character sets
    std::string allChars;
    for (const auto& provider : pImpl->providers) {
        allChars += provider->getCharacters();
    }
    
    if (allChars.empty()) {
        throw std::runtime_error("No characters available for generation");
    }
    
    std::string password;
    password.reserve(length);
    
    // Ensure at least one character from each set
    for (const auto& provider : pImpl->providers) {
        if (password.length() < length) {
            const std::string& chars = provider->getCharacters();
            if (!chars.empty()) {
                password += chars[pImpl->rng->generate(0, chars.length() - 1)];
            }
        }
    }
    
    // Fill remaining with random characters
    while (password.length() < length) {
        password += allChars[pImpl->rng->generate(0, allChars.length() - 1)];
    }
    
    // Shuffle for better randomness
    for (size_t i = password.length() - 1; i > 0; --i) {
        size_t j = pImpl->rng->generate(0, i);
        std::swap(password[i], password[j]);
    }
    
    return password;
}

} // namespace strategies
} // namespace password_generator