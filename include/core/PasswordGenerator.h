#pragma once

#include "core/config/PasswordGeneratorConfig.h"
#include "core/interfaces/IPasswordStrategy.h"
#include "core/interfaces/IPasswordValidator.h"
#include <string>
#include <memory>
#include <vector>

namespace password_generator {
namespace core {

/**
 * @brief Main facade class for password generation
 */
class PasswordGenerator {
public:
    /**
     * @brief Constructor
     * @param config The configuration for the generator
     */
    explicit PasswordGenerator(const config::PasswordGeneratorConfig& config = {});
    
    /**
     * @brief Destructor
     */
    ~PasswordGenerator();
    
    /**
     * @brief Set the password generation strategy
     * @param strategy The strategy to use
     */
    void setStrategy(std::unique_ptr<interfaces::IPasswordStrategy> strategy);
    
    /**
     * @brief Add a password validator
     * @param validator The validator to add
     */
    void addValidator(std::unique_ptr<interfaces::IPasswordValidator> validator);
    
    /**
     * @brief Generate a password
     * @return The generated password
     */
    std::string generate();
    
    /**
     * @brief Generate multiple passwords
     * @param count The number of passwords to generate
     * @return A vector of generated passwords
     */
    std::vector<std::string> generateBatch(size_t count);
    
    /**
     * @brief Validate a password against all registered validators
     * @param password The password to validate
     * @return true if the password passes all validators, false otherwise
     */
    bool validate(const std::string& password) const;
    
    /**
     * @brief Alias for validate() - Validate a password against all registered validators
     * @param password The password to validate
     * @return true if the password passes all validators, false otherwise
     */
    bool validatePassword(const std::string& password) const {
        return validate(password);
    }
    
    /**
     * @brief Get validation errors for a password
     * @param password The password to validate
     * @return A vector of error messages from failed validators
     */
    std::vector<std::string> getValidationErrors(const std::string& password) const;
    
    /**
     * @brief Get the current configuration
     * @return Reference to the configuration
     */
    const config::PasswordGeneratorConfig& getConfig() const;
    
    /**
     * @brief Set the configuration
     * @param config The new configuration
     */
    void setConfig(const config::PasswordGeneratorConfig& config);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace core
} // namespace password_generator
