#include <gtest/gtest.h>
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include "validators/EntropyValidator.h"
#include "strategies/PatternPasswordStrategy.h"
#include <thread>
#include <set>
#include <regex>
#include <chrono>

using namespace password_generator;
using namespace password_generator::core;

class PasswordGeneratorIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset to default config for each test
        defaultConfig = config::PasswordGeneratorConfig{};
    }
    
    config::PasswordGeneratorConfig defaultConfig;
};

// ============================================================================
// Basic Integration Tests
// ============================================================================

TEST_F(PasswordGeneratorIntegrationTest, GeneratesValidPasswordsWithDefaultConfig) {
    PasswordGenerator generator(defaultConfig);
    
    for (int i = 0; i < 100; ++i) {
        std::string password = generator.generate();
        
        // Verify length
        EXPECT_EQ(password.length(), defaultConfig.length);
        
        // Verify it's valid according to configured rules
        EXPECT_TRUE(generator.validatePassword(password));
        
        // Verify character types
        bool hasLower = false, hasUpper = false, hasDigit = false;
        for (char c : password) {
            if (std::islower(c)) hasLower = true;
            if (std::isupper(c)) hasUpper = true;
            if (std::isdigit(c)) hasDigit = true;
        }
        
        EXPECT_TRUE(hasLower) << "Password missing lowercase: " << password;
        EXPECT_TRUE(hasUpper) << "Password missing uppercase: " << password;
        EXPECT_TRUE(hasDigit) << "Password missing digits: " << password;
    }
}

TEST_F(PasswordGeneratorIntegrationTest, GeneratesUniquePasswords) {
    PasswordGenerator generator(defaultConfig);
    std::set<std::string> passwords;
}