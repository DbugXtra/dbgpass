#include "cli/commands/ActionCommands.h"
#include "cli/commands/CommandContext.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

int ConfigShowCommand::execute(CommandContext& context) {
    context.showConfig();
    return 0;
}

int GenerateCommand::execute(CommandContext& context) {
    // Validate configuration
    if (!context.config.includeLowercase && !context.config.includeUppercase &&
        !context.config.includeDigits && !context.config.includeSymbols) {
        std::cerr << "Error: At least one character type must be enabled\n";
        return 1;
    }

    context.generator.setConfig(context.config);
    std::string password = context.generator.generate();

    if (!context.quietMode) {
        std::cout << "\n┌─ Generated Password ─────────────────┐\n";
        std::cout << "│ " << std::setw(36) << std::left << password << " │\n";
        std::cout << "├──────────────────────────────────────┤\n";
        std::cout << "│ Length: " << std::setw(28) << std::left
                  << (std::to_string(password.length()) + " characters") << " │\n";

        // Calculate entropy
        size_t charSpace = 0;
        if (context.config.includeLowercase) charSpace += 26;
        if (context.config.includeUppercase) charSpace += 26;
        if (context.config.includeDigits) charSpace += 10;
        if (context.config.includeSymbols) charSpace += context.config.customSymbols.length();

        if (charSpace > 0) {
            double entropy = password.length() * std::log2(charSpace);
            std::cout << "│ Entropy: " << std::setw(28) << std::left
                      << (std::to_string(static_cast<int>(entropy)) + " bits") << " │\n";
        }

        std::cout << "└──────────────────────────────────────┘\n";
    } else {
        std::cout << password << "\n";
    }

    return 0;
}

std::unique_ptr<BatchCommand> BatchCommand::create(CommandContext& context) {
    if (!context.hasNextArg()) {
        std::cerr << "Error: --batch requires a count argument\n";
        return nullptr;
    }

    try {
        const std::string& countStr = context.getNextArg();
        size_t batchCount = std::stoul(countStr);
        if (batchCount == 0 || batchCount > 100) {
            std::cerr << "Error: Batch count must be between 1 and 100\n";
            return nullptr;
        }
        return std::make_unique<BatchCommand>(batchCount);
    } catch (const std::exception&) {
        std::cerr << "Error: Invalid batch count\n";
        return nullptr;
    }
}

int BatchCommand::execute(CommandContext& context) {
    // Validate configuration
    if (!context.config.includeLowercase && !context.config.includeUppercase &&
        !context.config.includeDigits && !context.config.includeSymbols) {
        std::cerr << "Error: At least one character type must be enabled\n";
        return 1;
    }

    context.generator.setConfig(context.config);
    auto passwords = context.generator.generateBatch(batchCount);

    if (!context.quietMode) {
        std::cout << "\n┌─ Generated " << batchCount << " Passwords ────────────\n";
        for (size_t i = 0; i < passwords.size(); ++i) {
            std::cout << "│ " << std::setw(3) << std::right << (i + 1) << ". "
                      << std::setw(30) << std::left << passwords[i] << "\n";
        }
    } else {
        for (const auto& password : passwords) {
            std::cout << password << "\n";
        }
    }

    return 0;
}

std::unique_ptr<ValidateCommand> ValidateCommand::create(CommandContext& context) {
    if (!context.hasNextArg()) {
        std::cerr << "Error: --validate requires a password argument\n";
        return nullptr;
    }

    const std::string& password = context.getNextArg();
    return std::make_unique<ValidateCommand>(password);
}

int ValidateCommand::execute(CommandContext& context) {
    // Validate configuration
    if (!context.config.includeLowercase && !context.config.includeUppercase &&
        !context.config.includeDigits && !context.config.includeSymbols) {
        std::cerr << "Error: At least one character type must be enabled\n";
        return 1;
    }

    context.generator.setConfig(context.config);
    auto errors = context.generator.getValidationErrors(password);

    if (errors.empty()) {
        if (!context.quietMode) {
            std::cout << "✓ Password is valid!\n";
        } else {
            std::cout << "valid\n";
        }
    } else {
        if (!context.quietMode) {
            std::cout << "✗ Password validation failed:\n";
            for (const auto& error : errors) {
                std::cout << "  - " << error << "\n";
            }
        } else {
            for (const auto& error : errors) {
                std::cout << error << "\n";
            }
        }
        return 1;
    }

    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator