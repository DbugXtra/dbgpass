#ifndef PRONOUNCEABLE_PASSWORD_STRATEGY_H
#define PRONOUNCEABLE_PASSWORD_STRATEGY_H

#include "core/interfaces/IPasswordStrategy.h"
#include "core/interfaces/IRandomGenerator.h"
#include <memory>

namespace password_generator {
namespace strategies {

/**
 * @brief Generates pronounceable passwords using syllables
 */
class PronounceablePasswordStrategy : public core::interfaces::IPasswordStrategy {
public:
    explicit PronounceablePasswordStrategy(
        std::unique_ptr<core::interfaces::IRandomGenerator> randomGen = nullptr);
    ~PronounceablePasswordStrategy();
    
    /**
     * @brief Generate pronounceable password
     */
    std::string generate(size_t length) override;
    
    /**
     * @brief Set whether to include numbers
     */
    void setIncludeNumbers(bool include);
    
    /**
     * @brief Set whether to include capitalization
     */
    void setIncludeCapitals(bool include);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace strategies
} // namespace password_generator

#endif // PRONOUNCEABLE_PASSWORD_STRATEGY_H