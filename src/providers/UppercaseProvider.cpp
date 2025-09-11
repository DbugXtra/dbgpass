#include "providers/UppercaseProvider.h"

namespace password_generator {
namespace providers {

std::string UppercaseProvider::getCharacters() const {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

std::string UppercaseProvider::getName() const {
    return "Uppercase";
}

} // namespace providers
} // namespace password_generator