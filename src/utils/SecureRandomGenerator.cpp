#include "utils/SecureRandomGenerator.h"
#include <random>

namespace password_generator {
namespace utils {

class SecureRandomGenerator::Impl {
public:
    std::random_device rd;
    std::mt19937 gen;
    
    Impl() : gen(rd()) {}
    
    void reseed() {
        gen.seed(rd());
    }
};

SecureRandomGenerator::SecureRandomGenerator()
    : pImpl(std::make_unique<Impl>()) {}

SecureRandomGenerator::~SecureRandomGenerator() = default;

int SecureRandomGenerator::generate(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(pImpl->gen);
}

void SecureRandomGenerator::reseed() {
    pImpl->reseed();
}

} // namespace utils
} // namespace password_generator