#pragma once

#include <string>
#include <memory>

namespace password_generator {
namespace core {
namespace interfaces {

/**
 * @brief Interface for providing character sets for password generation
 */
class ICharacterSetProvider {
public:
    virtual ~ICharacterSetProvider() = default;
    
    /**
     * @brief Get the character set provided by this provider
     * @return A string containing all available characters
     */
    virtual std::string getCharacters() const = 0;
    
    /**
     * @brief Get a human-readable name of this character set
     * @return The name of the character set (e.g., "Lowercase", "Digits")
     */
    virtual std::string getName() const = 0;
};

} // namespace interfaces
} // namespace core
} // namespace password_generator
