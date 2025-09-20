#include "cli/commands/ActionCommands.h"
#include "cli/commands/CommandContext.h"
#include <iostream>
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

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