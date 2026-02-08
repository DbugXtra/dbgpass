#pragma once

#include <string>
#include <memory>

namespace password_generator {
namespace core {
namespace interfaces {

/**
 * @brief Interface for password validation rules
 */
class IPasswordValidator {
public:
    virtual ~IPasswordValidator() = default;
    
    /**
     * @brief Validate a password
     * @param password The password to validate
     * @return true if the password meets this validation rule, false otherwise
     */
    virtual bool validate(const std::string& password) const = 0;
    
    /**
     * @brief Get an error message describing the validation failure
     * @return A human-readable error message
     */
    virtual std::string getErrorMessage() const = 0;
};

} // namespace interfaces
} // namespace core
} // namespace password_generator
