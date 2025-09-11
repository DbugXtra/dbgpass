#ifndef SYMBOL_PROVIDER_H
#define SYMBOL_PROVIDER_H

#include "core/interfaces/ICharacterSetProvider.h"
#include <string>

namespace password_generator {
namespace providers {

/**
 * @brief Provides symbol characters
 */
class SymbolProvider : public core::interfaces::ICharacterSetProvider {
public:
    explicit SymbolProvider(const std::string& symbols = "!@#$%^&*()_+-=[]{}|;:,.<>?");
    
    std::string getCharacters() const override;
    std::string getName() const override;
    
    /**
     * @brief Set custom symbol set
     */
    void setSymbols(const std::string& symbols);

private:
    std::string symbols_;
};

} // namespace providers
} // namespace password_generator

#endif // SYMBOL_PROVIDER_H