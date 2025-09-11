#include <gtest/gtest.h>
#include "validators/MinLengthValidator.h"
#include "validators/MaxLengthValidator.h"
#include "validators/CharacterTypeValidator.h"

using namespace password_generator::validators;

TEST(MinLengthValidatorTest, ValidatesMinimumLength) {
    MinLengthValidator validator(8);
    
    EXPECT_TRUE(validator.validate("12345678"));
    EXPECT_TRUE(validator.validate("123456789"));
    EXPECT_FALSE(validator.validate("1234567"));
    EXPECT_FALSE(validator.validate(""));
}

TEST(MaxLengthValidatorTest, ValidatesMaximumLength) {
    MaxLengthValidator validator(20);
    
    EXPECT_TRUE(validator.validate("12345678901234567890"));
    EXPECT_TRUE(validator.validate("short"));
    EXPECT_FALSE(validator.validate("123456789012345678901"));
}

TEST(CharacterTypeValidatorTest, ValidatesCharacterTypes) {
    CharacterTypeValidator validator(true, true, true, true);
    
    EXPECT_TRUE(validator.validate("Abc123!@#"));
    EXPECT_FALSE(validator.validate("abc123!@#"));  // No uppercase
    EXPECT_FALSE(validator.validate("ABC123!@#"));  // No lowercase
    EXPECT_FALSE(validator.validate("Abc!@#"));     // No digits
    EXPECT_FALSE(validator.validate("Abc123"));     // No symbols
}