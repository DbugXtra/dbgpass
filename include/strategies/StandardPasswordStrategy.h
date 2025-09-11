#ifndef STANDARD_PASSWORD_STRATEGY_H
#define STANDARD_PASSWORD_STRATEGY_H

#include "core/interfaces/IPasswordStrategy.h"
#include "core/interfaces/ICharacterSetProvider.h"
#include "core/interfaces/IRandomGenerator.h"
#include <memory>
#include <vector>

namespace password_generator {
namespace strategies {

/**
 * @brief Standard password generation strategy using character sets
 */
class StandardPasswordStrategy : public core::interfaces::IPasswordStrategy {
public:
    explicit StandardPasswordStrategy(
        std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
    ~StandardPasswordStrategy();
    
    /**
     * @brief Add a character set to use for generation
     */
    void addCharacterSet(std::unique_ptr<core::interfaces::ICharacterSetProvider> provider);
    
    /**
     * @brief Clear all character sets
     */
    void clearCharacterSets();
    
    /**
     * @brief Generate password
     */
    std::string generate(size_t length) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace strategies
} // namespace password_generator

#endif // STANDARD_PASSWORD_STRATEGY_H