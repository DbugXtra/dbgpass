#include "validators/MaxLengthValidator.h"

namespace password_generator {
namespace validators {

MaxLengthValidator::MaxLengthValidator(size_t maxLength)
    : maxLength_(maxLength) {}

bool MaxLengthValidator::validate(const std::string& password) const {
    return password.length() <= maxLength_;
}

std::string MaxLengthValidator::getErrorMessage() const {
    return "Password must not exceed " + std::to_string(maxLength_) + " characters";
}

void MaxLengthValidator::setMaxLength(size_t length) {
    maxLength_ = length;
}

size_t MaxLengthValidator::getMaxLength() const {
    return maxLength_;
}

} // namespace validators
} // namespace password_generator