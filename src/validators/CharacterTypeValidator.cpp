#include "validators/CharacterTypeValidator.h"
#include <cctype>

namespace password_generator {
namespace validators {

CharacterTypeValidator::CharacterTypeValidator(
    bool requireUpper, bool requireLower, bool requireDigit, bool requireSymbol)
    : requireUpper_(requireUpper), requireLower_(requireLower),
      requireDigit_(requireDigit), requireSymbol_(requireSymbol) {}

bool CharacterTypeValidator::validate(const std::string& password) const {
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;
    
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else hasSymbol = true;
    }
    
    return (!requireUpper_ || hasUpper) &&
           (!requireLower_ || hasLower) &&
           (!requireDigit_ || hasDigit) &&
           (!requireSymbol_ || hasSymbol);
}

std::string CharacterTypeValidator::getErrorMessage() const {
    std::string msg = "Password must contain";
    bool first = true;
    
    if (requireUpper_) {
        msg += first ? ": " : ", ";
        msg += "uppercase letters";
        first = false;
    }
    if (requireLower_) {
        msg += first ? ": " : ", ";
        msg += "lowercase letters";
        first = false;
    }
    if (requireDigit_) {
        msg += first ? ": " : ", ";
        msg += "digits";
        first = false;
    }
    if (requireSymbol_) {
        msg += first ? ": " : ", ";
        msg += "symbols";
        first = false;
    }
    
    return msg;
}

void CharacterTypeValidator::setRequireUppercase(bool require) {
    requireUpper_ = require;
}

void CharacterTypeValidator::setRequireLowercase(bool require) {
    requireLower_ = require;
}

void CharacterTypeValidator::setRequireDigit(bool require) {
    requireDigit_ = require;
}

void CharacterTypeValidator::setRequireSymbol(bool require) {
    requireSymbol_ = require;
}

} // namespace validators
} // namespace password_generator