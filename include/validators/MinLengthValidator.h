#ifndef MIN_LENGTH_VALIDATOR_H
#define MIN_LENGTH_VALIDATOR_H

#include "core/interfaces/IPasswordValidator.h"
#include <cstddef>

namespace password_generator {
namespace validators {

/**
 * @brief Validates minimum password length
 */
class MinLengthValidator : public core::interfaces::IPasswordValidator {
public:
    explicit MinLengthValidator(size_t minLength);
    
    bool validate(const std::string& password) const override;
    std::string getErrorMessage() const override;
    
    void setMinLength(size_t length);
    size_t getMinLength() const;

private:
    size_t minLength_;
};

} // namespace validators
} // namespace password_generator

#endif // MIN_LENGTH_VALIDATOR_H