#include "providers/DigitProvider.h"

namespace password_generator {
namespace providers {

std::string DigitProvider::getCharacters() const {
    return "0123456789";
}

std::string DigitProvider::getName() const {
    return "Digits";
}

} // namespace providers
} // namespace password_generator