#include "cli/commands/ActionCommands.h"
#include "cli/commands/CommandContext.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace password_generator {
namespace cli {
namespace commands {

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

} // namespace commands
} // namespace cli
} // namespace password_generator