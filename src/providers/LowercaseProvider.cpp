#include "providers/LowercaseProvider.h"

namespace password_generator {
namespace providers {

std::string LowercaseProvider::getCharacters() const {
    return "abcdefghijklmnopqrstuvwxyz";
}

std::string LowercaseProvider::getName() const {
    return "Lowercase";
}

} // namespace providers
} // namespace password_generator