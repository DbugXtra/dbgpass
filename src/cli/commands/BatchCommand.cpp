#include "cli/commands/ActionCommands.h"
#include "cli/commands/CommandContext.h"
#include <iostream>
#include <iomanip>
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

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

} // namespace commands
} // namespace cli
} // namespace password_generator