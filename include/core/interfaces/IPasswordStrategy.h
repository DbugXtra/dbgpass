#pragma once

#include <string>
#include <memory>

namespace password_generator {
namespace core {
namespace interfaces {

/**
 * @brief Interface for password generation strategies
 */
class IPasswordStrategy {
public:
    virtual ~IPasswordStrategy() = default;
    
    /**
     * @brief Generate a password of the specified length
     * @param length The desired password length
     * @return The generated password string
     */
    virtual std::string generate(size_t length) = 0;
};

} // namespace interfaces
} // namespace core
} // namespace password_generator
