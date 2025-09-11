#ifndef UPPERCASE_PROVIDER_H
#define UPPERCASE_PROVIDER_H

#include "core/interfaces/ICharacterSetProvider.h"

namespace password_generator {
namespace providers {

/**
 * @brief Provides uppercase letters A-Z
 */
class UppercaseProvider : public core::interfaces::ICharacterSetProvider {
public:
    std::string getCharacters() const override;
    std::string getName() const override;
};

} // namespace providers
} // namespace password_generator

#endif // UPPERCASE_PROVIDER_H