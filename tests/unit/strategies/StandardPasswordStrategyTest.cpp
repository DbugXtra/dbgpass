#include <gtest/gtest.h>
#include "strategies/StandardPasswordStrategy.h"
#include "providers/LowercaseProvider.h"
#include "providers/UppercaseProvider.h"
#include "providers/DigitProvider.h"
#include "mocks/MockRandomGenerator.h"

using namespace password_generator::strategies;
using namespace password_generator::providers;
using namespace password_generator::tests;

TEST(StandardPasswordStrategyTest, GeneratesWithAllCharacterSets) {
    auto mockRng = std::make_unique<MockRandomGenerator>(
        std::vector<int>{0, 0, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1});
    
    StandardPasswordStrategy strategy(std::move(mockRng));
    strategy.addCharacterSet(std::make_unique<LowercaseProvider>());
    strategy.addCharacterSet(std::make_unique<UppercaseProvider>());
    strategy.addCharacterSet(std::make_unique<DigitProvider>());
    
    std::string password = strategy.generate(10);
    
    EXPECT_EQ(password.length(), 10);
    
    // Should contain at least one from each set
    bool hasLower = false, hasUpper = false, hasDigit = false;
    
    for (char c : password) {
        if (std::islower(c)) hasLower = true;
        if (std::isupper(c)) hasUpper = true;
        if (std::isdigit(c)) hasDigit = true;
    }
    
    EXPECT_TRUE(hasLower);
    EXPECT_TRUE(hasUpper);
    EXPECT_TRUE(hasDigit);
}

TEST(StandardPasswordStrategyTest, ThrowsWhenNoCharacterSets) {
    StandardPasswordStrategy strategy;
    
    EXPECT_THROW(strategy.generate(10), std::runtime_error);
}