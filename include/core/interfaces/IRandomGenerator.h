#pragma once

#include <cstdint>
#include <memory>

namespace password_generator {
namespace core {
namespace interfaces {

/**
 * @brief Interface for random number generation
 */
class IRandomGenerator {
public:
    virtual ~IRandomGenerator() = default;
    
    /**
     * @brief Generate a random number in the range [min, max)
     * @param min The lower bound (inclusive)
     * @param max The upper bound (exclusive)
     * @return A random number in the specified range
     */
    virtual int generate(int min, int max) = 0;
};

} // namespace interfaces
} // namespace core
} // namespace password_generator
