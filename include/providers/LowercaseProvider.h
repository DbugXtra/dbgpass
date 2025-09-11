#ifndef LOWERCASE_PROVIDER_H
#define LOWERCASE_PROVIDER_H

#include "core/interfaces/ICharacterSetProvider.h"

namespace password_generator {
namespace providers {

/**
 * @brief Provides lowercase letters a-z
 */
class LowercaseProvider : public core::interfaces::ICharacterSetProvider {
public:
    std::string getCharacters() const override;
    std::string getName() const override;
};

} // namespace providers
} // namespace password_generator

#endif // LOWERCASE_PROVIDER_H