#ifndef MAX_LENGTH_VALIDATOR_H
#define MAX_LENGTH_VALIDATOR_H

#include "core/interfaces/IPasswordValidator.h"
#include <cstddef>

namespace password_generator {
namespace validators {

/**
 * @brief Validates maximum password length
 */
class MaxLengthValidator : public core::interfaces::IPasswordValidator {
public:
    explicit MaxLengthValidator(size_t maxLength);
    
    bool validate(const std::string& password) const override;
    std::string getErrorMessage() const override;
    
    void setMaxLength(size_t length);
    size_t getMaxLength() const;

private:
    size_t maxLength_;
};

} // namespace validators
} // namespace password_generator

#endif // MAX_LENGTH_VALIDATOR_H