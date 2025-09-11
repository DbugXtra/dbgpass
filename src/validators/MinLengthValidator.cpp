#include "validators/MinLengthValidator.h"

namespace password_generator {
namespace validators {

MinLengthValidator::MinLengthValidator(size_t minLength)
    : minLength_(minLength) {}

bool MinLengthValidator::validate(const std::string& password) const {
    return password.length() >= minLength_;
}

std::string MinLengthValidator::getErrorMessage() const {
    return "Password must be at least " + std::to_string(minLength_) + " characters";
}

void MinLengthValidator::setMinLength(size_t length) {
    minLength_ = length;
}

size_t MinLengthValidator::getMinLength() const {
    return minLength_;
}

} // namespace validators
} // namespace password_generator