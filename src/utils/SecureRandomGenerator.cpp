#include "utils/SecureRandomGenerator.h"
#include <random>
#include <stdexcept>
#include <cstdint>

namespace password_generator {
namespace utils {

class SecureRandomGenerator::Impl {
public:
    std::random_device rd;
    
    Impl() {
        // Verify random_device entropy
        if (rd.entropy() == 0) {
            throw std::runtime_error("Hardware random number generator not available");
        }
    }
    
    void reseed() {
        // Random device doesn't need reseeding - each call is independent
    }
};

SecureRandomGenerator::SecureRandomGenerator()
    : pImpl(std::make_unique<Impl>()) {}

SecureRandomGenerator::~SecureRandomGenerator() = default;

int SecureRandomGenerator::generate(int min, int max) {
    if (min > max) {
        throw std::invalid_argument("min must be <= max");
    }
    
    // Use random_device directly for cryptographic security
    // Generate uniformly distributed random number in range [min, max]
    const uint32_t range = static_cast<uint32_t>(max - min + 1);
    const uint32_t max_valid = UINT32_MAX - (UINT32_MAX % range);
    
    uint32_t value;
    do {
        value = pImpl->rd();
    } while (value >= max_valid); // Reject bias-inducing values
    
    return min + (value % range);
}

void SecureRandomGenerator::reseed() {
    pImpl->reseed();
}

} // namespace utils
} // namespace password_generator