#ifndef CHARACTER_TYPE_VALIDATOR_H
#define CHARACTER_TYPE_VALIDATOR_H

#include "core/interfaces/IPasswordValidator.h"

namespace password_generator {
namespace validators {

/**
 * @brief Validates presence of required character types
 */
class CharacterTypeValidator : public core::interfaces::IPasswordValidator {
public:
    CharacterTypeValidator(bool requireUpper = true, bool requireLower = true,
                          bool requireDigit = true, bool requireSymbol = false);
    
    bool validate(const std::string& password) const override;
    std::string getErrorMessage() const override;
    
    void setRequireUppercase(bool require);
    void setRequireLowercase(bool require);
    void setRequireDigit(bool require);
    void setRequireSymbol(bool require);

private:
    bool requireUpper_;
    bool requireLower_;
    bool requireDigit_;
    bool requireSymbol_;
};

} // namespace validators
} // namespace password_generator

#endif // CHARACTER_TYPE_VALIDATOR_H