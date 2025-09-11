#include "providers/SymbolProvider.h"

namespace password_generator {
namespace providers {

SymbolProvider::SymbolProvider(const std::string& symbols)
    : symbols_(symbols) {}

std::string SymbolProvider::getCharacters() const {
    return symbols_;
}

std::string SymbolProvider::getName() const {
    return "Symbols";
}

void SymbolProvider::setSymbols(const std::string& symbols) {
    symbols_ = symbols;
}

} // namespace providers
} // namespace password_generator