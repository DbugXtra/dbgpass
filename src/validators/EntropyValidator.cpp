#include "validators/EntropyValidator.h"
#include <cmath>
#include <unordered_map>

namespace password_generator {
namespace validators {

EntropyValidator::EntropyValidator(double minEntropy)
    : minEntropy_(minEntropy) {}

bool EntropyValidator::validate(const std::string& password) const {
    double entropy = calculateEntropy(password);
    return entropy >= minEntropy_;
}

std::string EntropyValidator::getErrorMessage() const {
    return "Password entropy must be at least " + std::to_string(minEntropy_) + " bits";
}

void EntropyValidator::setMinEntropy(double entropy) {
    minEntropy_ = entropy;
}

double EntropyValidator::getMinEntropy() const {
    return minEntropy_;
}

double EntropyValidator::calculateEntropy(const std::string& password) const {
    if (password.empty()) {
        return 0.0;
    }
    
    // Count character frequencies
    std::unordered_map<char, int> frequencies;
    for (char c : password) {
        frequencies[c]++;
    }
    
    // Calculate Shannon entropy
    double entropy = 0.0;
    double length = static_cast<double>(password.length());
    
    for (const auto& pair : frequencies) {
        double probability = static_cast<double>(pair.second) / length;
        entropy -= probability * std::log2(probability);
    }
    
    return entropy * length;
}

} // namespace validators
} // namespace password_generator