#ifndef ENTROPY_VALIDATOR_H
#define ENTROPY_VALIDATOR_H

#include "core/interfaces/IPasswordValidator.h"
#include <cstddef>

namespace password_generator {
namespace validators {

/**
 * @brief Validates password entropy (randomness)
 */
class EntropyValidator : public core::interfaces::IPasswordValidator {
public:
    explicit EntropyValidator(double minEntropy);
    
    bool validate(const std::string& password) const override;
    std::string getErrorMessage() const override;
    
    void setMinEntropy(double entropy);
    double getMinEntropy() const;

private:
    double calculateEntropy(const std::string& password) const;
    
    double minEntropy_;
};

} // namespace validators
} // namespace password_generator

#endif // ENTROPY_VALIDATOR_H