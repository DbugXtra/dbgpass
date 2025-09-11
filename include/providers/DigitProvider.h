#ifndef DIGIT_PROVIDER_H
#define DIGIT_PROVIDER_H

#include "core/interfaces/ICharacterSetProvider.h"

namespace password_generator {
namespace providers {

/**
 * @brief Provides digits 0-9
 */
class DigitProvider : public core::interfaces::ICharacterSetProvider {
public:
    std::string getCharacters() const override;
    std::string getName() const override;
};

} // namespace providers
} // namespace password_generator

#endif // DIGIT_PROVIDER_H