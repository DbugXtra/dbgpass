#ifndef PATTERN_PASSWORD_STRATEGY_H
#define PATTERN_PASSWORD_STRATEGY_H

#include "core/interfaces/IPasswordStrategy.h"
#include "core/interfaces/IRandomGenerator.h"
#include <memory>
#include <string>

namespace password_generator {
namespace strategies {

/**
 * @brief Pattern-based password generation strategy
 * 
 * Generates passwords based on a pattern where:
 * - 'L' = lowercase letter
 * - 'U' = uppercase letter  
 * - 'D' = digit
 * - 'S' = symbol
 * - Other characters are used literally
 */
class PatternPasswordStrategy : public core::interfaces::IPasswordStrategy {
public:
    explicit PatternPasswordStrategy(
        const std::string& pattern,
        std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
    ~PatternPasswordStrategy();
    
    /**
     * @brief Set the password pattern
     */
    void setPattern(const std::string& pattern);
    
    /**
     * @brief Get the current pattern
     */
    const std::string& getPattern() const;
    
    /**
     * @brief Generate password based on pattern
     */
    std::string generate(size_t length) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace strategies
} // namespace password_generator

#endif // PATTERN_PASSWORD_STRATEGY_H