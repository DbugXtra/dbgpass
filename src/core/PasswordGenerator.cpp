#include "core/PasswordGenerator.h"
#include <algorithm>
#include <stdexcept>

namespace password_generator {
namespace core {

class PasswordGenerator::Impl {
public:
    config::PasswordGeneratorConfig config;
    std::unique_ptr<interfaces::IPasswordStrategy> strategy;
    std::vector<std::unique_ptr<interfaces::IPasswordValidator>> validators;
    
    Impl(const config::PasswordGeneratorConfig& cfg) : config(cfg) {}
};

PasswordGenerator::PasswordGenerator(const config::PasswordGeneratorConfig& config)
    : pImpl(std::make_unique<Impl>(config)) {
}

PasswordGenerator::~PasswordGenerator() = default;

void PasswordGenerator::setStrategy(std::unique_ptr<interfaces::IPasswordStrategy> strategy) {
    pImpl->strategy = std::move(strategy);
}

void PasswordGenerator::addValidator(std::unique_ptr<interfaces::IPasswordValidator> validator) {
    pImpl->validators.push_back(std::move(validator));
}

std::string PasswordGenerator::generate() {
    if (!pImpl->strategy) {
        throw std::runtime_error("No password generation strategy set");
    }
    return pImpl->strategy->generate(pImpl->config.length);
}

std::vector<std::string> PasswordGenerator::generateBatch(size_t count) {
    std::vector<std::string> passwords;
    passwords.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        passwords.push_back(generate());
    }
    return passwords;
}

bool PasswordGenerator::validate(const std::string& password) const {
    for (const auto& validator : pImpl->validators) {
        if (!validator->validate(password)) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> PasswordGenerator::getValidationErrors(const std::string& password) const {
    std::vector<std::string> errors;
    for (const auto& validator : pImpl->validators) {
        if (!validator->validate(password)) {
            errors.push_back(validator->getErrorMessage());
        }
    }
    return errors;
}

const config::PasswordGeneratorConfig& PasswordGenerator::getConfig() const {
    return pImpl->config;
}

void PasswordGenerator::setConfig(const config::PasswordGeneratorConfig& config) {
    pImpl->config = config;
}

} // namespace core
} // namespace password_generator
