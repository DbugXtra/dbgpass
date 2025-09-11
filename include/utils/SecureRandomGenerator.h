#ifndef SECURE_RANDOM_GENERATOR_H
#define SECURE_RANDOM_GENERATOR_H

#include "core/interfaces/IRandomGenerator.h"
#include <memory>
#include <random>

namespace password_generator {
namespace utils {

/**
 * @brief Cryptographically secure random number generator
 */
class SecureRandomGenerator : public core::interfaces::IRandomGenerator {
public:
    SecureRandomGenerator();
    virtual ~SecureRandomGenerator();
    
    int generate(int min, int max) override;
    
    /**
     * @brief Re-seed the generator
     */
    void reseed();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace utils
} // namespace password_generator

#endif // SECURE_RANDOM_GENERATOR_H